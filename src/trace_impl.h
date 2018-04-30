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

#ifndef LTTLWHWHT_TRACE_IMPL_H
#define LTTLWHWHT_TRACE_IMPL_H

#include "trace.h"

#include "kernelsystem.h"
#include "androidsystem.h"

#include <vector>
#include <string>
#include <memory>
#include <cstdio>

using namespace std;

class TraceImpl : public Trace {
  public:
    ~TraceImpl();
    // Set all the kernel tracing settings to the desired state for this trace
    // capture.
    bool setUp() override;
    // Reset all the kernel tracing settings to their default state.
    void cleanUp() override;
    // Enable tracing in the kernel.
    bool start() override;
    // Disable tracing in the kernel.
    void stop() override;
    void setErrorStream(FILE * errorStream);
    void setAndroidSystem(AndroidSystem * androidSystem);
    void setKernelSystem(KernelSystem * kernelSystem);
    void enableTraceOverwrite();
    void setTraceBufferSizeKB(uint32_t size);
    void addKernelCategory(const char* categoryName);
    void addAndroidCategory(const char * categoryName);
    void addApp(const char * appName);
    void addFunction(const char * funcName);
  private:
    FILE * m_ErrorStream = NULL;
    AndroidSystem *_AndroidSystem;
    KernelSystem *_KernelSystem;
    bool m_TraceOverwriteSwitch = false;
    uint32_t m_TraceBufferSizeKB = 2048;
    vector<string> m_KernelCategories;
    vector<string> m_AndroidCategories;
    vector<string> m_Apps;
    vector<string> m_Functions;
};

#endif // LTTLWHWHT_TRACE_IMPL_H