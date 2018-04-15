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
#include <fcntl.h>  // creat, ope
#include <stdlib.h> // free
#include <unistd.h> // read, close
#include <set>

KernelSystemImpl::~KernelSystemImpl()
{
    delete this->file_system;
    delete this->toolbox;
}

void KernelSystemImpl::set_errstream(FILE * errstream)
{
    this->errstream = errstream;
}

void KernelSystemImpl::set_file_system(FileSystem * file_system)
{
    this->file_system = file_system;
}

void KernelSystemImpl::set_toolbox(Toolbox * toolbox)
{
    this->toolbox = toolbox;
}

bool KernelSystemImpl::setKernelOptionEnable(const char* filename, bool enable)
{
    return file_system->writeStr(filename, enable ? "1" : "0");
}

bool KernelSystemImpl::isPossibleSetKernelOption(const char* filename)
{
    return filename != NULL && file_system->fileIsWritable(filename);
}

bool KernelSystemImpl::isCategorySupported(const TracingCategory& category)
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

bool KernelSystemImpl::isCategorySupportedForRoot(const TracingCategory& category)
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

bool KernelSystemImpl::setKernelTraceFuncs(const char* commasepfuncs)
{
    bool ok = true;

    if (commasepfuncs == NULL || commasepfuncs[0] == '\0') {
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
        std::set<std::string> funcs;
        toolbox->parseToTokens(commasepfuncs, ",", funcs);
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

bool KernelSystemImpl::verifyKernelTraceFuncs(const std::set<std::string> & funcs) const
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