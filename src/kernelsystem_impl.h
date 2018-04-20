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

#ifndef LTTLWHWHT_KERNELSYSTEM_IMPL_H
#define LTTLWHWHT_KERNELSYSTEM_IMPL_H

#include "kernelsystem.h"
#include "filesystem.h"
#include "toolbox.h"
#include "systemtime.h"

#include <stdio.h>
#include <map>

using namespace std;

class KernelSystemImpl : public KernelSystem {
  public:
    ~KernelSystemImpl();
    void set_errstream(FILE * errstream);
    void set_file_system(FileSystem * file_system);
    void set_toolbox(Toolbox * toolbox);
    void set_systime(SystemTime * systime);
    int tryOpenToWriteOrCreate(const char* filename);
    bool try_sendfile(int fd_from, int fd_to);
    bool writeClockSyncMarker() override;
    bool setTraceOverwriteEnable(bool enable) override;
    bool setTracingEnabled(bool enable) override;
    bool clearTrace() override;
    int getTracePipeFd() override;
    int getTraceFd() override;
    bool try_send(int fd_from, int fd_to) override;
    bool setTraceBufferSizeKB(int size) override;
    // Enable or disable the kernel's use of the global clock.  Disabling the global
    // clock will result in the kernel using a per-CPU local clock.
    // Any write to the trace_clock sysfs file will reset the buffer, so only
    // update it if the requested value is not the current value.
    bool setGlobalClockEnable(bool enable) override;
    bool setPrintTgidEnableIfPresent(bool enable) override;
    // Set the comma separated list of functions that the kernel is to trace.
    bool setKernelTraceFuncs(const char* funcs) override;
    bool enableKernelTraceEvents(const vector<string> & ids);
    const vector<TracingCategory> & getCategories() const override;
    // Disable all /sys/ enable files.
    bool disableKernelTraceEvents();
    void add_kernel_category(const char * id, const char * name, const std::vector<EnableFile> &files);
    bool isCategorySupported(const TracingCategory& category) const;
  private:
    FILE * errstream = NULL;
    FileSystem * file_system = NULL;
    Toolbox * toolbox = NULL;
    SystemTime * systime = NULL;

    map<string, TracingCategory> m_Categories;
    vector<TracingCategory> m_CategoriesList;

    bool writeMarker(const char * buffer);
    // Read the trace_clock sysfs file and return true if it matches the requested
    // value.  The trace_clock file format is:
    // local [global] counter uptime perf
    bool isTraceClock(const char *mode);
    bool setKernelOptionEnable(const char* filename, bool enable);
    bool isPossibleSetKernelOption(const char* filename);
    bool verifyKernelTraceFuncs(const std::set<std::string> & funcs) const;
    // Check whether the category is supported on the device with the current
    // rootness.  A category is supported only if all its required /sys/ files are
    // writable and if enabling the category will enable one or more tracing tags
    // or /sys/ files.
    bool _isCategorySupported(const TracingCategory& category) const;
    // Check whether the category would be supported on the device if the user
    // were root.  This function assumes that root is able to write to any file
    // that exists.  It performs the same logic as isCategorySupported, but it
    // uses file existance rather than writability in the /sys/ file checks.
    bool isCategorySupportedForRoot(const TracingCategory& category) const;
    const char * k_traceClockPath =  "/sys/kernel/debug/tracing/trace_clock";
    const char * k_traceBufferSizePath =  "/sys/kernel/debug/tracing/buffer_size_kb";
    const char * k_tracingOverwriteEnablePath =  "/sys/kernel/debug/tracing/options/overwrite";
    const char * k_currentTracerPath =  "/sys/kernel/debug/tracing/current_tracer";
    const char * k_printTgidPath =  "/sys/kernel/debug/tracing/options/print-tgid";
    const char * k_funcgraphAbsTimePath =  "/sys/kernel/debug/tracing/options/funcgraph-abstime";
    const char * k_funcgraphCpuPath =  "/sys/kernel/debug/tracing/options/funcgraph-cpu";
    const char * k_funcgraphProcPath =  "/sys/kernel/debug/tracing/options/funcgraph-proc";
    // const char * k_funcgraphFlatPath =  "/sys/kernel/debug/tracing/options/funcgraph-flat";
    const char * k_ftraceFilterPath =  "/sys/kernel/debug/tracing/set_ftrace_filter";
    const char * k_tracingOnPath =  "/sys/kernel/debug/tracing/tracing_on";
    const char * k_tracePath =  "/sys/kernel/debug/tracing/trace";
    const char * k_traceStreamPath =  "/sys/kernel/debug/tracing/trace_pipe";
    const char * k_traceMarkerPath =  "/sys/kernel/debug/tracing/trace_marker";
};

#endif // LTTLWHWHT_KERNELSYSTEM_IMPL_H