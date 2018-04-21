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

#include <zlib.h>

using namespace android;

//! Number of app packages to trace
#define MAX_PACKAGES 16

void AndroidSystemImpl::set_errstream(FILE * errstream) {
    this->errstream = errstream;
}

void AndroidSystemImpl::add_category(const char * id, const char * name, uint64_t atrace_tag) {
    m_Categories[id] = { id, name, atrace_tag, {}, false };
    m_CategoriesList.push_back({ id, name, atrace_tag, {}, false });
}

bool AndroidSystemImpl::has_core_services() const {
    char value[PROPERTY_VALUE_MAX];
    property_get(k_coreServicesProp, value, "");
    return strlen(value) != 0;
}

void AndroidSystemImpl::property_get_core_service_names(std::string & content) const {
    char value[PROPERTY_VALUE_MAX];
    property_get(k_coreServicesProp, value, "");
    content += value;
}
bool AndroidSystemImpl::setAppCmdlineProperty(const vector<string> & appNames) {
    if (appNames.size() > MAX_PACKAGES) {
        fprintf(errstream, "error: only 16 packages could be traced at once\n");
        clearAppProperties();
        return false;
    }
    char buf[PROPERTY_KEY_MAX];
    for (size_t i = 0; i < appNames.size(); i++) {
        snprintf(buf, sizeof(buf), k_traceAppsPropertyTemplate, i);
        if (property_set(buf, appNames[i].c_str()) < 0) {
            fprintf(errstream, "error setting trace app %d property to %s\n", i, buf);
            clearAppProperties();
            return false;
        }
    }
    snprintf(buf, sizeof(buf), "%d", appNames.size());
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

void AndroidSystemImpl::log_dumping_trace() {
    ALOGI("Dumping trace");
}

const std::vector<TracingCategory> & AndroidSystemImpl::getCategories() const {
    return m_CategoriesList;
}

bool AndroidSystemImpl::tryEnableCategories(const vector<string> & categories) {
    uint64_t tags = 0;
    for (const auto & id : categories) {
        if (m_Categories.find(id) == m_Categories.end()) {
            fprintf(errstream, "category is not supported - %s", id.c_str());
            return false;
        }
        tags |= m_Categories[id].tags;
    }
    return setTagsProperty(tags);
}

void AndroidSystemImpl::disableAllCategories() {
  setTagsProperty(0);
}