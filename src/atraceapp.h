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

#ifndef LTTLWHWHT_ATRACEAPP_H
#define LTTLWHWHT_ATRACEAPP_H

#include "androidsystem.h"
#include "kernelsystem.h"
#include "tracingcategory.h"
#include "toolbox.h"
#include "trace.h"

#include <stdio.h>    // FILE
#include <inttypes.h> // uint64_t
#include <vector>
#include <memory>

using namespace std;

class AtraceApp {
  public:
    ~AtraceApp();
    void handleSignal();
    void setTrace(Trace * trace);
    void set_android_system(shared_ptr<AndroidSystem> & android_system);
    void set_kernel_system(shared_ptr<KernelSystem> & kernel_system);
    void set_errstream(FILE * errstream);
    void set_outstream(FILE * outstream);
    void nosignals();
    void set_initialSleepSecs(int secs);
    void set_traceDurationSeconds(int secs);
    void enable_compression();
    void set_outputFile(const char * filename);
    void set_async(bool option);
    void set_stop(bool option);
    void set_dump(bool option);
    void set_start(bool option);
    void enable_streaming();
    bool run();
  private:
    std::vector<std::string> m_KernelCategories;
    std::vector<std::string> androidCategories;
    std::vector<std::string> m_Apps;
    std::vector<std::string> m_Functions;

    Trace * m_Trace = NULL;
    shared_ptr<KernelSystem> kernel_system;
    shared_ptr<AndroidSystem> android_system;

    FILE * errstream = NULL;
    FILE * outstream = NULL;

    /* Command line options */
    bool async = false;
    bool traceStart = true;
    bool traceStop = true;
    bool traceDump = true;
    bool traceStream = false;
    int g_traceDurationSeconds = 5;
    bool g_compress = false;
    bool g_nohup = false;
    int g_initialSleepSecs = 0;
    std::string g_kernelTraceFuncs;
    std::string g_outputFile;
    /* Global state */
    bool g_traceAborted = false;

    // Read data from the tracing pipe and forward to outstream
    void streamTrace();
    // Read the current kernel trace and write it to outstream.
    void dumpTrace();
};

#endif // LTTLWHWHT_ATRACEAPP_H