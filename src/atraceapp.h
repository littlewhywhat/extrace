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
#include "systemtime.h"
#include "tracingcategory.h"

#include <stdio.h>    // FILE
#include <inttypes.h> // uint64_t
#include <vector>

class AtraceApp {
  public:
    ~AtraceApp();
    void handleSignal();
    void set_android_system(AndroidSystem * android_system);
    void set_kernel_system(KernelSystem * kernel_system);
    void set_systime(SystemTime * systime);
    void set_errstream(FILE * errstream);
    void set_outstream(FILE * outstream);
    void add_android_category(const char * id, const char * name, uint64_t atrace_tag);
    void add_kernel_category(const char * id, const char * name, const std::vector<EnableFile> &sysfiles);
    void set_android_core_services(const char * id, const char * name);
    int run_atrace(int argc, char **argv);
  private:
    std::vector<TracingCategory> k_categories;
    SystemTime * systime = NULL;
    KernelSystem * kernel_system = NULL;
    AndroidSystem * android_system = NULL;

    FILE * errstream = NULL;
    FILE * outstream = NULL;

    const char* k_coreServiceCategory = NULL;

    /* Command line options */
    int g_traceDurationSeconds = 5;
    bool g_traceOverwrite = false;
    int g_traceBufferSizeKB = 2048;
    bool g_compress = false;
    bool g_nohup = false;
    int g_initialSleepSecs = 0;
    const char* g_categoriesFile = NULL;
    const char* g_kernelTraceFuncs = NULL;
    const char* g_debugAppCmdLine = "";
    const char* g_outputFile = nullptr;

    /* Global state */
    bool g_traceAborted = false;

    bool writeClockSyncMarker();
    // Check whether the category is supported on the device with the current
    // rootness.  A category is supported only if all its required /sys/ files are
    // writable and if enabling the category will enable one or more tracing tags
    // or /sys/ files.
    bool isCategorySupported(const TracingCategory& category);
    // Disable all /sys/ enable files.
    bool disableKernelTraceEvents();
    bool setCategoryEnable(const char* name, bool enable);
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
    void dumpTrace(int outFd);
    void listSupportedCategories();
    // Print the command usage help to errstream.
    void showHelp(const char *cmd);
};

#endif // LTTLWHWHT_ATRACEAPP_H