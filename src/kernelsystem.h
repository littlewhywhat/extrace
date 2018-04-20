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

#ifndef LTTLWHWHT_KERNELSYSTEM_H
#define LTTLWHWHT_KERNELSYSTEM_H

#include "tracingcategory.h"
#include <set>
#include <string>
#include <stdio.h>

using namespace std;

class KernelSystem {
  public:
    virtual ~KernelSystem() {}
    virtual int tryOpenToWriteOrCreate(const char* filename) = 0;
    virtual bool try_sendfile(int fd_from, int fd_to) = 0;
    virtual bool writeClockSyncMarker() = 0;
    // Enable or disable overwriting of the kernel trace buffers.  Disabling this
    // will cause tracing to stop once the trace buffers have filled up.
    virtual bool setTraceOverwriteEnable(bool enable) = 0;
    // Enable or disable kernel tracing.
    virtual bool setTracingEnabled(bool enable) = 0;
    virtual bool clearTrace() = 0;
    virtual int getTracePipeFd() = 0;
    virtual int getTraceFd() = 0;
    virtual bool try_send(int fd_from, int fd_to) = 0;
    virtual bool setTraceBufferSizeKB(int size) = 0;
    virtual bool setGlobalClockEnable(bool enable) = 0;
    virtual bool setPrintTgidEnableIfPresent(bool enable) = 0;
    virtual bool setKernelTraceFuncs(const vector<string> & funcs) = 0;
    virtual bool enableKernelTraceEvents(const vector<string> & ids) = 0;
    virtual const vector<TracingCategory> & getCategories() const = 0;
    // Disable all /sys/ enable files.
    virtual bool disableKernelTraceEvents() = 0;
    // Enable or disable a kernel option by writing a "1" or a"0" into a /sys
    // file.
    virtual bool isCategorySupported(const TracingCategory& category) const = 0;
};

#endif // LTTLWHWHT_KERNELSYSTEM_H