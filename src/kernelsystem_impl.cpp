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

#include "kernelsystem_impl.h"

#include <errno.h>  // errno
#include <string.h> // strerror
#include <fcntl.h>  // creat, ope, O_WRONLY, O_CREAT
#include <stdlib.h> // free
#include <unistd.h> // read, close
#include <stdio.h>  // FILE
#include <set>
#include <sys/sendfile.h>
#include <zlib.h>

KernelSystemImpl::~KernelSystemImpl()
{}

void KernelSystemImpl::set_errstream(FILE * errstream)
{
    this->errstream = errstream;
}

void KernelSystemImpl::set_systime(shared_ptr<SystemTime> & systime) {
    this->systime = systime;
}

void KernelSystemImpl::set_file_system(shared_ptr<FileSystem> & file_system)
{
    this->file_system = file_system;
}

void KernelSystemImpl::set_toolbox(shared_ptr<ToolBox> & toolbox)
{
    this->toolbox = toolbox;
}

int KernelSystemImpl::tryOpenToWriteOrCreate(const char* filename)
{
    int outFd = open(filename, O_WRONLY | O_CREAT);
    if (outFd == -1) {
      printf("Failed to open '%s', err=%d", filename, errno);
    }
    return outFd;
}

bool KernelSystemImpl::try_sendfile(int fd_from, int fd_to)
{
    ssize_t sent = 0;
    while ((sent = sendfile(fd_to, fd_from, NULL, 64*1024*1024)) > 0);
    if (sent == -1) {
        fprintf(errstream, "error sendfile: %s (%d)\n", strerror(errno),
                errno);
        return false;
    }
    return true;
}

bool KernelSystemImpl::try_send(int fd_from, int fd_to) {
    char trace_data[4096];
    ssize_t bytes_read = read(fd_from, trace_data, 4096);
    if (bytes_read > 0) {
        write(fd_to, trace_data, bytes_read);
        return true;
    }
    return false;
}

bool KernelSystemImpl::setKernelOptionEnable(const char* filename, bool enable)
{
    return file_system->writeStr(filename, enable ? "1" : "0");
}

bool KernelSystemImpl::isPossibleSetKernelOption(const char* filename)
{
    return filename != NULL && file_system->fileIsWritable(filename);
}

bool KernelSystemImpl::isCategorySupported(const TracingCategory& category) const
{
    if (_isCategorySupported(category)) {
        return true;
    } else {
        if (isCategorySupportedForRoot(category)) {
            fprintf(errstream, "error: category \"%s\" requires root "
                    "privileges.\n", category.name);
        } else {
            fprintf(errstream, "error: category \"%s\" is not supported "
                    "on this device.\n", category.name);
        }
        return false;
    }
}

bool KernelSystemImpl::_isCategorySupported(const TracingCategory& category) const
{
    bool ok = true;
    for (const auto & file : category.files) {
        const char* path = file.path;
        bool req = file.required == EnableFile::REQ;
        if (path != NULL) {
            if (req) {
                if (!file_system->fileIsWritable(path)) {
                    fprintf(errstream, "File %s is not writable\n", path);
                    return false;
                } else {
                    ok = true;
                }
            } else {
                ok |= file_system->fileIsWritable(path);
            }
        }
    }
    return ok;
}

bool KernelSystemImpl::isCategorySupportedForRoot(const TracingCategory& category) const
{
    bool ok = category.tags != 0;
    for (const auto & file : category.files) {
        const char* path = file.path;
        bool req = file.required == EnableFile::REQ;
        if (path != NULL) {
            if (req) {
                if (!file_system->fileExists(path)) {
                    return false;
                } else {
                    ok = true;
                }
            } else {
                ok |= file_system->fileExists(path);
            }
        }
    }
    return ok;
}

bool KernelSystemImpl::writeClockSyncMarker()
{
  printf("writeClockSyncMarker\n");
  char buffer[128];
  float now_in_seconds = systime->get_monotonic();
  snprintf(buffer, 128, "trace_event_clock_sync: parent_ts=%f\n", now_in_seconds);
  bool ok = true;
  ok &= writeMarker(buffer);
  int64_t realtime_in_ms = systime->get_realtime();
  snprintf(buffer, 128, "trace_event_clock_sync: realtime_ts=%" PRId64 "\n", realtime_in_ms);
  ok &= writeMarker(buffer);
  return ok;
}

bool KernelSystemImpl::writeMarker(const char * buffer)
{
  return file_system->writeStr(k_traceMarkerPath, buffer);
}

bool KernelSystemImpl::setTraceOverwriteEnable(bool enable)
{
  return setKernelOptionEnable(k_tracingOverwriteEnablePath, enable);
}

bool KernelSystemImpl::setTracingEnabled(bool enable)
{
    return setKernelOptionEnable(k_tracingOnPath, enable);
}

bool KernelSystemImpl::clearTrace()
{
    return file_system->truncateFile(k_tracePath);
}

int KernelSystemImpl::getTracePipeFd()
{
    int traceFD = open(k_traceStreamPath, O_RDWR);
    if (traceFD == -1) {
       fprintf(errstream, "error opening %s: %s (%d)\n", k_traceStreamPath,
                strerror(errno), errno);
    }
    return traceFD;
}

int KernelSystemImpl::getTraceFd()
{
    int traceFD = open(k_tracePath, O_RDWR);
    if (traceFD == -1) {
       fprintf(errstream, "error opening %s: %s (%d)\n", k_tracePath,
                strerror(errno), errno);
    }
    return traceFD;
}

bool KernelSystemImpl::setTraceBufferSizeKB(int size)
{
    char str[32] = "1";
    if (size < 1) {
        size = 1;
    }
    snprintf(str, 32, "%d", size);
    return file_system->writeStr(k_traceBufferSizePath, str);
}

bool KernelSystemImpl::setGlobalClockEnable(bool enable)
{
    const char *clock = enable ? "global" : "local";

    if (isTraceClock(clock)) {
        return true;
    }

    return file_system->writeStr(k_traceClockPath, clock);
}

bool KernelSystemImpl::setPrintTgidEnableIfPresent(bool enable)
{
    if (file_system->fileExists(k_printTgidPath)) {
        return setKernelOptionEnable(k_printTgidPath, enable);
    }
    return true;
}

bool KernelSystemImpl::setKernelTraceFuncs(const vector<string> & funcs)
{
    bool ok = true;

    if (funcs.empty()) {
        // Disable kernel function tracing.
        if (file_system->fileIsWritable(k_currentTracerPath)) {
            ok &= file_system->writeStr(k_currentTracerPath, "nop");
        }
        if (file_system->fileIsWritable(k_ftraceFilterPath)) {
            ok &= file_system->truncateFile(k_ftraceFilterPath);
        }
    } else {
        // Enable kernel function tracing.
        ok &= file_system->writeStr(k_currentTracerPath, "function_graph");
        ok &= setKernelOptionEnable(k_funcgraphAbsTimePath, true);
        ok &= setKernelOptionEnable(k_funcgraphCpuPath, true);
        ok &= setKernelOptionEnable(k_funcgraphProcPath, true);
        // ok &= setKernelOptionEnable(k_funcgraphFlatPath, true);

        // Set the requested filter functions.
        ok &= file_system->truncateFile(k_ftraceFilterPath);
        for (const auto & func: funcs) {
            ok &= file_system->appendStr(k_ftraceFilterPath, func.c_str());
        }

        // Verify that the set functions are being traced.
        if (ok) {
            ok &= verifyKernelTraceFuncs(funcs);
        }
    }

    return ok;
}

bool KernelSystemImpl::isTraceClock(const char *mode)
{
    char buf[4097];    
    if (!file_system->readStr(k_traceClockPath, buf, 4097)) {
        return false;
    }

    char *start = strchr(buf, '[');
    if (start == NULL) {
        return false;
    }
    start++;

    char *end = strchr(start, ']');
    if (end == NULL) {
        return false;
    }
    *end = '\0';

    return strcmp(mode, start) == 0;
}

bool KernelSystemImpl::verifyKernelTraceFuncs(const vector<string> & funcs) const
{
    char buf[4097];    
    if (!file_system->readStr(k_ftraceFilterPath, buf, 4097)) {
        return false;
    }

    std::set<std::string> funcs_traced;
    toolbox->parseToTokens(buf, "\n", funcs_traced);

    bool ok = true;
    for (const auto & func : funcs) {
        // except wildcards
        if (strchr(func.c_str(), '*') == NULL 
            && funcs_traced.find(func) == funcs_traced.end()) {
            fprintf(errstream, "error: \"%s\" is not a valid kernel function to trace.\n",
                    func.c_str());
            ok = false;
        }
    }
    return ok;
}

void KernelSystemImpl::add_kernel_category(const char * id, const char * name, const std::vector<EnableFile> &files)
{
  m_Categories[id] = {id, name, 0, files, false };
  m_CategoriesList.push_back({id, name, 0, files, false });
}

// Disable all /sys/ enable files.
bool KernelSystemImpl::disableKernelTraceEvents() {
    bool ok = true;
    for (const auto & category : m_CategoriesList) {
        for (const auto & file : category.files) {
            const char* path = file.path;
            if (isPossibleSetKernelOption(path)) {
                ok &= setKernelOptionEnable(path, false);
            }
        }
    }
    return ok;
}

bool KernelSystemImpl::enableKernelTraceEvents(const std::vector<string> & ids) {
    bool ok = true;
    for (const auto & id : ids) {
        if (m_Categories.find(id) == m_Categories.end()) {
            fprintf(errstream, "wrong kernel category id '%s'\n", id.c_str());
            return false;
        } else {
            const auto & category = m_Categories[id];
            for (const auto & file : category.files) {
                const char* path = file.path;
                bool required = file.required == EnableFile::REQ;
                if (path != NULL) {
                    if (isPossibleSetKernelOption(path)) {
                        ok &= setKernelOptionEnable(path, true);
                    } else if (required) {
                        fprintf(errstream, "error writing file %s\n", path);
                        ok = false;
                    }
                }
            }
        }
    }
    return ok;
}

const vector<TracingCategory> & KernelSystemImpl::getCategories() const {
    return m_CategoriesList;
}

bool KernelSystemImpl::compress_trace_to(int traceFD, int outFd) {
    z_stream zs;
    uint8_t *in, *out;
    int result, flush;

    memset(&zs, 0, sizeof(zs));
    result = deflateInit(&zs, Z_DEFAULT_COMPRESSION);
    if (result != Z_OK) {
        fprintf(errstream, "error initializing zlib: %d\n", result);
        close(traceFD);
        return false;
    }

    bool ok = true;
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
                ok = false;
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
                ok = false;
                break;
            }
            zs.next_out = out;
            zs.avail_out = bufSize;
        }

    } while ((result = deflate(&zs, flush)) == Z_OK);

    if (result != Z_STREAM_END) {
        fprintf(errstream, "error deflating trace: %s\n", zs.msg);
        ok = false;
    }

    if (zs.avail_out < bufSize) {
        size_t bytes = bufSize - zs.avail_out;
        result = write(outFd, out, bytes);
        if ((size_t)result < bytes) {
            fprintf(errstream, "error writing deflated trace: %s (%d)\n",
                    strerror(errno), errno);
            ok = false;
        }
    }

    result = deflateEnd(&zs);
    if (result != Z_OK) {
        fprintf(errstream, "error cleaning up zlib: %d\n", result);
        ok = false;
    }

    free(in);
    free(out);
    return ok;
}