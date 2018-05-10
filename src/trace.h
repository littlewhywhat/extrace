/*
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

#ifndef LTTLWHWHT_TRACE_H
#define LTTLWHWHT_TRACE_H

#include "signal.h"
#include "android.h"
#include "kerneltracesystem.h"

#include <string>

using namespace std;

class Trace {
  public:
    virtual ~Trace() {}
    // virtual void addKernelCategory(const KernelTraceSystem::TraceCategory & category) = 0;
    // virtual void addAndroidCategory(const Android::TraceCategory & categoryName) = 0;
    // virtual void addApp(const char * appName) = 0;
    // virtual void addFunction(const char * funcName) = 0;
    // Set all the kernel tracing settings to the desired state for this trace
    // capture.
    virtual bool setUp() = 0;
    // Reset all the kernel tracing settings to their default state.
    virtual bool cleanUp() = 0;
    // Enable tracing in the kernel.
    virtual bool start() = 0;
    // Disable tracing in the kernel.
    virtual bool stop() = 0;
};

#endif // LTTLWHWHT_TRACE_H