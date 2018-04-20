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

#include <stdio.h>    // FILE
#include <inttypes.h> // uint64_t
#include <vector>

class AtraceApp {
  public:
    ~AtraceApp();
    void handleSignal();
    void set_android_system(AndroidSystem * android_system);
    void set_kernel_system(KernelSystem * kernel_system);
    void set_errstream(FILE * errstream);
    void set_outstream(FILE * outstream);
    void set_toolbox(Toolbox * toolbox);
    void set_debugAppCmdLine(const char * app);
    void set_traceBufferSizeKB(int size);
    void enable_trace_overwrite();
    void set_kernelTraceFuncs(const char * funcs);
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
    void listSupportedCategories();
    bool setCategory(const char* name);
    void add_android_category(const char * id);
    void add_kernel_category(const char * id, const char * name, const std::vector<EnableFile> &sysfiles);
    void enableAndroidCore();
    bool run();
  private:
    std::vector<TracingCategory> k_categories;
    std::vector<std::string> androidCategories;
    Toolbox * toolbox = NULL;
    KernelSystem * kernel_system = NULL;
    AndroidSystem * android_system = NULL;

    FILE * errstream = NULL;
    FILE * outstream = NULL;

    /* Command line options */
    bool async = false;
    bool traceStart = true;
    bool traceStop = true;
    bool traceDump = true;
    bool traceStream = false;
    int g_traceDurationSeconds = 5;
    bool g_traceOverwrite = false;
    int g_traceBufferSizeKB = 2048;
    bool g_compress = false;
    bool g_nohup = false;
    int g_initialSleepSecs = 0;
    std::string g_kernelTraceFuncs;
    std::string g_debugAppCmdLine;
    std::string g_outputFile;
    bool enableCoreServices = false;
    /* Global state */
    bool g_traceAborted = false;

    
    // Check whether the category is supported on the device with the current
    // rootness.  A category is supported only if all its required /sys/ files are
    // writable and if enabling the category will enable one or more tracing tags
    // or /sys/ files.
    bool isCategorySupported(const TracingCategory& category);
    // Disable all /sys/ enable files.
    bool disableKernelTraceEvents();
    // Set all the kernel tracing settings to the desired state for this trace
    // capture.
    bool setUpTrace();
    // Reset all the kernel tracing settings to their default state.
    void cleanUpTrace();
    // Enable tracing in the kernel.
    bool startTrace();
    // Disable tracing in the kernel.
    void stopTrace();
    // Read data from the tracing pipe and forward to outstream
    void streamTrace();
    // Read the current kernel trace and write it to outstream.
    void dumpTrace();
};

#endif // LTTLWHWHT_ATRACEAPP_H