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

class KernelSystem {
  public:
    virtual ~KernelSystem() {}
    virtual bool setKernelOptionEnable(const char* filename, bool enable) = 0;
    virtual bool isPossibleSetKernelOption(const char * filename) = 0;
    virtual bool isCategorySupported(const TracingCategory& category) = 0;
    virtual bool isCategorySupportedForRoot(const TracingCategory& category) = 0;
    virtual bool writeMarker(const char * buffer) = 0;
    // Enable or disable overwriting of the kernel trace buffers.  Disabling this
    // will cause tracing to stop once the trace buffers have filled up.
    virtual bool setTraceOverwriteEnable(bool enable) = 0;
    // Enable or disable kernel tracing.
    virtual bool setTracingEnabled(bool enable) = 0;
    virtual bool clearTrace() = 0;
    virtual int getTracePipeFd() = 0;
    virtual int getTraceFd() = 0;
    virtual bool setTraceBufferSizeKB(int size) = 0;
    virtual bool setGlobalClockEnable(bool enable) = 0;
    virtual bool setPrintTgidEnableIfPresent(bool enable) = 0;
    virtual bool setKernelTraceFuncs(const char* funcs) = 0;
    virtual bool verifyKernelTraceFuncs(const std::set<std::string> & funcs) const = 0;
};

#endif // LTTLWHWHT_KERNELSYSTEM_H