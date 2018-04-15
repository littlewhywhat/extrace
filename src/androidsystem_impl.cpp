/*
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2018 Roman Vaivod
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "androidsystem_impl.h"

#include <binder/IBinder.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <cutils/properties.h>
#include <utils/String8.h>
// #include <utils/Tokenizer.h>
#include <zlib.h>

using namespace android;

//! Number of app packages to trace
#define MAX_PACKAGES 16

void AndroidSystemImpl::set_errstream(FILE * errstream) {
    this->errstream = errstream;
}

bool AndroidSystemImpl::has_core_services() const {
    char value[PROPERTY_VALUE_MAX];
    property_get(k_coreServicesProp, value, "");
    return strlen(value) != 0;
}

// bool AndroidSystemImpl::setCategoriesEnableFromFile(const char * categories_file) {
//     if (!categories_file) {
//         return true;
//     }
//     Tokenizer* tokenizer = NULL;
//     if (Tokenizer::open(String8(categories_file), &tokenizer) != NO_ERROR) {
//         return false;
//     }
//     bool ok = true;
//     while (!tokenizer->isEol()) {
//         String8 token = tokenizer->nextToken(" ");
//         if (token.isEmpty()) {
//             tokenizer->skipDelimiters(" ");
//             continue;
//         }
//         ok &= setCategoryEnable(token.string(), true);
//     }
//     delete tokenizer;
//     return ok;
// }

void AndroidSystemImpl::property_get_core_service_names(std::string & content) const {
    char value[PROPERTY_VALUE_MAX];
    property_get(k_coreServicesProp, value, "");
    content += value;
}
bool AndroidSystemImpl::setAppCmdlineProperty(const char * cmdline) {
    char buf[PROPERTY_KEY_MAX];
    int i = 0;
    const char* start = cmdline;
    while (start != NULL) {
        if (i == MAX_PACKAGES) {
            fprintf(errstream, "error: only 16 packages could be traced at once\n");
            clearAppProperties();
            return false;
        }
        char* end = strchr(start, ',');
        if (end != NULL) {
            *end = '\0';
            end++;
        }
        snprintf(buf, sizeof(buf), k_traceAppsPropertyTemplate, i);
        if (property_set(buf, start) < 0) {
            fprintf(errstream, "error setting trace app %d property to %s\n", i, buf);
            clearAppProperties();
            return false;
        }
        start = end;
        i++;
    }

    snprintf(buf, sizeof(buf), "%d", i);
    if (property_set(k_traceAppsNumberProperty, buf) < 0) {
        fprintf(errstream, "error setting trace app number property to %s\n", buf);
        clearAppProperties();
        return false;
    }
    return true;
}
bool AndroidSystemImpl::pokeBinderServices() {
    sp<IServiceManager> sm = defaultServiceManager();
    Vector<String16> services = sm->listServices();
    for (size_t i = 0; i < services.size(); i++) {
        sp<IBinder> obj = sm->checkService(services[i]);
        if (obj != NULL) {
            Parcel data;
            if (obj->transact(IBinder::SYSPROPS_TRANSACTION, data,
                    NULL, 0) != OK) {
                if (false) {
                    // XXX: For some reason this fails on tablets trying to
                    // poke the "phone" service.  It's not clear whether some
                    // are expected to fail.
                    String8 svc(services[i]);
                    fprintf(errstream, "error poking binder service %s\n",
                        svc.string());
                    return false;
                }
            }
        }
    }
    return true;
}
bool AndroidSystemImpl::setTagsProperty(uint64_t tags) {
    char buf[PROPERTY_VALUE_MAX];
    snprintf(buf, sizeof(buf), "%#" PRIx64, tags);
    if (property_set(k_traceTagsProperty, buf) < 0) {
        fprintf(errstream, "error setting trace tags system property\n");
        return false;
    }
    return true;
}
void AndroidSystemImpl::clearAppProperties() {
    char buf[PROPERTY_KEY_MAX];
    for (int i = 0; i < MAX_PACKAGES; i++) {
        snprintf(buf, sizeof(buf), k_traceAppsPropertyTemplate, i);
        if (property_set(buf, "") < 0) {
            fprintf(errstream, "failed to clear system property: %s\n", buf);
        }
    }
    if (property_set(k_traceAppsNumberProperty, "") < 0) {
        fprintf(errstream, "failed to clear system property: %s",
              k_traceAppsNumberProperty);
    }
}
void AndroidSystemImpl::compress_trace_to(int traceFD, int outFd) {
    z_stream zs;
    uint8_t *in, *out;
    int result, flush;

    memset(&zs, 0, sizeof(zs));
    result = deflateInit(&zs, Z_DEFAULT_COMPRESSION);
    if (result != Z_OK) {
        fprintf(errstream, "error initializing zlib: %d\n", result);
        close(traceFD);
        return;
    }

    const size_t bufSize = 64*1024;
    in = (uint8_t*)malloc(bufSize);
    out = (uint8_t*)malloc(bufSize);
    flush = Z_NO_FLUSH;

    zs.next_out = out;
    zs.avail_out = bufSize;

    do {

        if (zs.avail_in == 0) {
            // More input is needed.
            result = read(traceFD, in, bufSize);
            if (result < 0) {
                fprintf(errstream, "error reading trace: %s (%d)\n",
                        strerror(errno), errno);
                result = Z_STREAM_END;
                break;
            } else if (result == 0) {
                flush = Z_FINISH;
            } else {
                zs.next_in = in;
                zs.avail_in = result;
            }
        }

        if (zs.avail_out == 0) {
            // Need to write the output.
            result = write(outFd, out, bufSize);
            if ((size_t)result < bufSize) {
                fprintf(errstream, "error writing deflated trace: %s (%d)\n",
                        strerror(errno), errno);
                result = Z_STREAM_END; // skip deflate error message
                zs.avail_out = bufSize; // skip the final write
                break;
            }
            zs.next_out = out;
            zs.avail_out = bufSize;
        }

    } while ((result = deflate(&zs, flush)) == Z_OK);

    if (result != Z_STREAM_END) {
        fprintf(errstream, "error deflating trace: %s\n", zs.msg);
    }

    if (zs.avail_out < bufSize) {
        size_t bytes = bufSize - zs.avail_out;
        result = write(outFd, out, bytes);
        if ((size_t)result < bytes) {
            fprintf(errstream, "error writing deflated trace: %s (%d)\n",
                    strerror(errno), errno);
        }
    }

    result = deflateEnd(&zs);
    if (result != Z_OK) {
        fprintf(errstream, "error cleaning up zlib: %d\n", result);
    }

    free(in);
    free(out);
}
void AndroidSystemImpl::log_dumping_trace() {
    ALOGI("Dumping trace");
}