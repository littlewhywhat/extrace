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
#include "atrace_abstract.cpp"

#define LOG_TAG "atrace"

#include <stdio.h>

#include <zlib.h> // compression

#include <binder/IBinder.h>          // pokeBinderServices
#include <binder/IServiceManager.h>  // pokeBinderServices
#include <binder/Parcel.h>           // pokeBinderServices

#include <cutils/properties.h> // property_get, property_set

#include <utils/String8.h>   // string manipulation
#include <utils/Timers.h>
#include <utils/Tokenizer.h> // string manipulation
#include <utils/Trace.h>     // ATRACE_TAGs

using namespace android;

class FileSystem {
  public:
    virtual ~FileSystem() {}
    virtual bool fileExists(const char* filename) = 0;
    virtual bool fileIsWritable(const char* filename) = 0;
    virtual bool truncateFile(const char* path) = 0;
    virtual bool writeStr(const char* filename, const char* str) = 0;
    virtual bool appendStr(const char* filename, const char* str) = 0;
};

class FileSystemImpl : public FileSystem {
  public:
    void set_errstream(FILE * errstream) {
        this->errstream = errstream;
    }

    // Check whether a file exists.
    bool fileExists(const char* filename) override
    {
        return access(filename, F_OK) != -1;
    }
    // Check whether a file is writable.
    bool fileIsWritable(const char* filename) override
    {
        return access(filename, W_OK) != -1;
    }
    // Truncate a file.
    bool truncateFile(const char* path) override
    {
        // This uses creat rather than truncate because some of the debug kernel
        // device nodes (e.g. k_ftraceFilterPath) currently aren't changed by
        // calls to truncate, but they are cleared by calls to creat.
        int traceFD = creat(path, 0);
        if (traceFD == -1) {
            fprintf(errstream, "error truncating %s: %s (%d)\n", path,
                strerror(errno), errno);
            return false;
        }

        close(traceFD);

        return true;
    }
    // Write a string to a file, returning true if the write was successful.
    bool writeStr(const char* filename, const char* str) override
    {
        return _writeStr(filename, str, O_WRONLY);
    }
    // Append a string to a file, returning true if the write was successful.
    bool appendStr(const char* filename, const char* str) override
    {
        return _writeStr(filename, str, O_APPEND|O_WRONLY);
    }
  private:
    FILE * errstream;
    bool _writeStr(const char* filename, const char* str, int flags)
    {
        int fd = open(filename, flags);
        if (fd == -1) {
            fprintf(errstream, "error opening %s: %s (%d)\n", filename,
                    strerror(errno), errno);
            return false;
        }

        bool ok = true;
        ssize_t len = strlen(str);
        if (write(fd, str, len) != len) {
            fprintf(errstream, "error writing to %s: %s (%d)\n", filename,
                    strerror(errno), errno);
            ok = false;
        }

        close(fd);

        return ok;
    }
};

class SystemTimeImpl : public SystemTime {
  public:
    float get_monotonic() const {
      return systemTime(CLOCK_MONOTONIC) / 1000000000.0f;
    }
    int64_t get_realtime() const {
      return systemTime(CLOCK_REALTIME) / 1000000;
    }
};

class KernelSystemImpl : public KernelSystem {
  public:
    ~KernelSystemImpl() { delete this->file_system; }
    void set_errstream(FILE * errstream) {
        this->errstream = errstream;
    }

    void set_file_system(FileSystem * file_system) {
        this->file_system = file_system;
    }
    // Enable or disable a kernel option by writing a "1" or a "0" into a /sys
    // file.
    bool setKernelOptionEnable(const char* filename, bool enable) override
    {
        return file_system->writeStr(filename, enable ? "1" : "0");
    }
    bool isPossibleSetKernelOption(const char* filename) override
    {
        return filename != NULL && file_system->fileIsWritable(filename);
    }

    // Check whether the category is supported on the device with the current
    // rootness.  A category is supported only if all its required /sys/ files are
    // writable and if enabling the category will enable one or more tracing tags
    // or /sys/ files.
    bool isCategorySupported(const TracingCategory& category) override
    {
        bool ok = true;
        for (int i = 0; i < MAX_SYS_FILES; i++) {
            const char* path = category.sysfiles[i].path;
            bool req = category.sysfiles[i].required == REQ;
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

    // Check whether the category would be supported on the device if the user
    // were root.  This function assumes that root is able to write to any file
    // that exists.  It performs the same logic as isCategorySupported, but it
    // uses file existance rather than writability in the /sys/ file checks.
    bool isCategorySupportedForRoot(const TracingCategory& category) override
    {
        bool ok = category.tags != 0;
        for (int i = 0; i < MAX_SYS_FILES; i++) {
            const char* path = category.sysfiles[i].path;
            bool req = category.sysfiles[i].required == REQ;
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
    bool writeMarker(const char * buffer) override
    {
      return file_system->writeStr(k_traceMarkerPath, buffer);
    }
    bool setTraceOverwriteEnable(bool enable) override
    {
      return setKernelOptionEnable(k_tracingOverwriteEnablePath, enable);
    }
    bool setTracingEnabled(bool enable) override
    {
        return setKernelOptionEnable(k_tracingOnPath, enable);
    }
    bool clearTrace() override
    {
        return file_system->truncateFile(k_tracePath);
    }
    int getTracePipeFd() override
    {
        int traceFD = open(k_traceStreamPath, O_RDWR);
        if (traceFD == -1) {
           fprintf(errstream, "error opening %s: %s (%d)\n", k_traceStreamPath,
                    strerror(errno), errno);
        }
        return traceFD;
    }
    int getTraceFd() override
    {
        int traceFD = open(k_tracePath, O_RDWR);
        if (traceFD == -1) {
           fprintf(errstream, "error opening %s: %s (%d)\n", k_tracePath,
                    strerror(errno), errno);
        }
        return traceFD;
    }
    bool setTraceBufferSizeKB(int size) override
    {
        char str[32] = "1";
        if (size < 1) {
            size = 1;
        }
        snprintf(str, 32, "%d", size);
        return file_system->writeStr(k_traceBufferSizePath, str);
    }

    // Enable or disable the kernel's use of the global clock.  Disabling the global
    // clock will result in the kernel using a per-CPU local clock.
    // Any write to the trace_clock sysfs file will reset the buffer, so only
    // update it if the requested value is not the current value.
    bool setGlobalClockEnable(bool enable) override
    {
        const char *clock = enable ? "global" : "local";

        if (isTraceClock(clock)) {
            return true;
        }

        return file_system->writeStr(k_traceClockPath, clock);
    }
    bool setPrintTgidEnableIfPresent(bool enable) override
    {
        if (file_system->fileExists(k_printTgidPath)) {
            return setKernelOptionEnable(k_printTgidPath, enable);
        }
        return true;
    }
    // Set the comma separated list of functions that the kernel is to trace.
    bool setKernelTraceFuncs(const char* funcs) override
    {
        bool ok = true;

        if (funcs == NULL || funcs[0] == '\0') {
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
            ok &= setKernelOptionEnable(k_funcgraphFlatPath, true);

            // Set the requested filter functions.
            ok &= file_system->truncateFile(k_ftraceFilterPath);
            char* myFuncs = strdup(funcs);
            char* func = strtok(myFuncs, ",");
            while (func) {
                ok &= file_system->appendStr(k_ftraceFilterPath, func);
                func = strtok(NULL, ",");
            }
            free(myFuncs);

            // Verify that the set functions are being traced.
            if (ok) {
                ok &= verifyKernelTraceFuncs(funcs);
            }
        }

        return ok;
    }

  private:
    FILE * errstream;
    FileSystem * file_system;
    // Read the trace_clock sysfs file and return true if it matches the requested
    // value.  The trace_clock file format is:
    // local [global] counter uptime perf
    bool isTraceClock(const char *mode)
    {
        int fd = open(k_traceClockPath, O_RDONLY);
        if (fd == -1) {
            fprintf(errstream, "error opening %s: %s (%d)\n", k_traceClockPath,
                strerror(errno), errno);
            return false;
        }

        char buf[4097];
        ssize_t n = read(fd, buf, 4096);
        close(fd);
        if (n == -1) {
            fprintf(errstream, "error reading %s: %s (%d)\n", k_traceClockPath,
                strerror(errno), errno);
            return false;
        }
        buf[n] = '\0';

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
    bool verifyKernelTraceFuncs(const char * funcs) const {
        int fd = open(k_ftraceFilterPath, O_RDONLY);
        if (fd == -1) {
            fprintf(errstream, "error opening %s: %s (%d)\n", k_ftraceFilterPath,
                strerror(errno), errno);
            return false;
        }

        char buf[4097];
        ssize_t n = read(fd, buf, 4096);
        close(fd);
        if (n == -1) {
            fprintf(errstream, "error reading %s: %s (%d)\n", k_ftraceFilterPath,
                strerror(errno), errno);
            return false;
        }

        buf[n] = '\0';
        String8 funcList = String8::format("\n%s", buf);

        // Make sure that every function listed in funcs is in the list we just
        // read from the kernel, except for wildcard inputs.
        bool ok = true;
        char* myFuncs = strdup(funcs);
        char* func = strtok(myFuncs, ",");
        while (func) {
            if (!strchr(func, '*')) {
                String8 fancyFunc = String8::format("\n%s\n", func);
                bool found = funcList.find(fancyFunc.string(), 0) >= 0;
                if (!found || func[0] == '\0') {
                    fprintf(errstream, "error: \"%s\" is not a valid kernel function "
                            "to trace.\n", func);
                    ok = false;
                }
            }
            func = strtok(NULL, ",");
        }
        free(myFuncs);

        return ok;
    }
    const char * k_traceClockPath =  "/sys/kernel/debug/tracing/trace_clock";
    const char * k_traceBufferSizePath =  "/sys/kernel/debug/tracing/buffer_size_kb";
    const char * k_tracingOverwriteEnablePath =  "/sys/kernel/debug/tracing/options/overwrite";
    const char * k_currentTracerPath =  "/sys/kernel/debug/tracing/current_tracer";
    const char * k_printTgidPath =  "/sys/kernel/debug/tracing/options/print-tgid";
    const char * k_funcgraphAbsTimePath =  "/sys/kernel/debug/tracing/options/funcgraph-abstime";
    const char * k_funcgraphCpuPath =  "/sys/kernel/debug/tracing/options/funcgraph-cpu";
    const char * k_funcgraphProcPath =  "/sys/kernel/debug/tracing/options/funcgraph-proc";
    const char * k_funcgraphFlatPath =  "/sys/kernel/debug/tracing/options/funcgraph-flat";
    const char * k_ftraceFilterPath =  "/sys/kernel/debug/tracing/set_ftrace_filter";
    const char * k_tracingOnPath =  "/sys/kernel/debug/tracing/tracing_on";
    const char * k_tracePath =  "/sys/kernel/debug/tracing/trace";
    const char * k_traceStreamPath =  "/sys/kernel/debug/tracing/trace_pipe";
    const char * k_traceMarkerPath =  "/sys/kernel/debug/tracing/trace_marker";
};

class AndroidSystemImpl : public AndroidSystem {
  public:
    void set_errstream(FILE * errstream) {
        this->errstream = errstream;
    }

    bool has_core_services() const {
        char value[PROPERTY_VALUE_MAX];
        property_get(k_coreServicesProp, value, "");
        return strlen(value) != 0;
    }

    // bool setCategoriesEnableFromFile(const char * categories_file) {
    //     if (!categories_file) {
    //         return true;
    //     }
    //     Tokenizer* tokenizer = NULL;
    //     if (Tokenizer::open(String8(categories_file), &tokenizer) != NO_ERROR) {
    //         return false;
    //     }
    //     bool ok = true;
    //     while (!tokenizer->isEol()) {
    //         String8 token = tokenizer->nextToken(" ");
    //         if (token.isEmpty()) {
    //             tokenizer->skipDelimiters(" ");
    //             continue;
    //         }
    //         ok &= setCategoryEnable(token.string(), true);
    //     }
    //     delete tokenizer;
    //     return ok;
    // }
    void property_get_core_service_names(std::string & content) const {
        char value[PROPERTY_VALUE_MAX];
        property_get(k_coreServicesProp, value, "");
        content += value;
    }
    bool setAppCmdlineProperty(const char * cmdline) {
        char buf[PROPERTY_KEY_MAX];
        int i = 0;
        const char* start = cmdline;
        while (start != NULL) {
            if (i == MAX_PACKAGES) {
                fprintf(errstream, "error: only 16 packages could be traced at once\n");
                clearAppProperties();
                return false;
            }
            char* end = strchr(start, ',');
            if (end != NULL) {
                *end = '\0';
                end++;
            }
            snprintf(buf, sizeof(buf), k_traceAppsPropertyTemplate, i);
            if (property_set(buf, start) < 0) {
                fprintf(errstream, "error setting trace app %d property to %s\n", i, buf);
                clearAppProperties();
                return false;
            }
            start = end;
            i++;
        }

        snprintf(buf, sizeof(buf), "%d", i);
        if (property_set(k_traceAppsNumberProperty, buf) < 0) {
            fprintf(errstream, "error setting trace app number property to %s\n", buf);
            clearAppProperties();
            return false;
        }
        return true;
    }
    bool pokeBinderServices() {
        sp<IServiceManager> sm = defaultServiceManager();
        Vector<String16> services = sm->listServices();
        for (size_t i = 0; i < services.size(); i++) {
            sp<IBinder> obj = sm->checkService(services[i]);
            if (obj != NULL) {
                Parcel data;
                if (obj->transact(IBinder::SYSPROPS_TRANSACTION, data,
                        NULL, 0) != OK) {
                    if (false) {
                        // XXX: For some reason this fails on tablets trying to
                        // poke the "phone" service.  It's not clear whether some
                        // are expected to fail.
                        String8 svc(services[i]);
                        fprintf(errstream, "error poking binder service %s\n",
                            svc.string());
                        return false;
                    }
                }
            }
        }
        return true;
    }
    bool setTagsProperty(uint64_t tags) {
        char buf[PROPERTY_VALUE_MAX];
        snprintf(buf, sizeof(buf), "%#" PRIx64, tags);
        if (property_set(k_traceTagsProperty, buf) < 0) {
            fprintf(errstream, "error setting trace tags system property\n");
            return false;
        }
        return true;
    }
    void clearAppProperties() {
        char buf[PROPERTY_KEY_MAX];
        for (int i = 0; i < MAX_PACKAGES; i++) {
            snprintf(buf, sizeof(buf), k_traceAppsPropertyTemplate, i);
            if (property_set(buf, "") < 0) {
                fprintf(errstream, "failed to clear system property: %s\n", buf);
            }
        }
        if (property_set(k_traceAppsNumberProperty, "") < 0) {
            fprintf(errstream, "failed to clear system property: %s",
                  k_traceAppsNumberProperty);
        }
    }
    void compress_trace_to(int traceFD, int outFd) {
        z_stream zs;
        uint8_t *in, *out;
        int result, flush;

        memset(&zs, 0, sizeof(zs));
        result = deflateInit(&zs, Z_DEFAULT_COMPRESSION);
        if (result != Z_OK) {
            fprintf(errstream, "error initializing zlib: %d\n", result);
            close(traceFD);
            return;
        }

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
                    break;
                }
                zs.next_out = out;
                zs.avail_out = bufSize;
            }

        } while ((result = deflate(&zs, flush)) == Z_OK);

        if (result != Z_STREAM_END) {
            fprintf(errstream, "error deflating trace: %s\n", zs.msg);
        }

        if (zs.avail_out < bufSize) {
            size_t bytes = bufSize - zs.avail_out;
            result = write(outFd, out, bytes);
            if ((size_t)result < bytes) {
                fprintf(errstream, "error writing deflated trace: %s (%d)\n",
                        strerror(errno), errno);
            }
        }

        result = deflateEnd(&zs);
        if (result != Z_OK) {
            fprintf(errstream, "error cleaning up zlib: %d\n", result);
        }

        free(in);
        free(out);
    }
    void log_dumping_trace() {
        ALOGI("Dumping trace");
    }
  private:
    FILE * errstream;
    const char* k_traceTagsProperty = "debug.atrace.tags.enableflags";
    const char* k_coreServicesProp = "ro.atrace.core.services";
    const char* k_traceAppsNumberProperty = "debug.atrace.app_number";
    const char* k_traceAppsPropertyTemplate = "debug.atrace.app_%d";
};

Atrace atrace;

void handleSignal(int /*signo*/)
{
    atrace.handleSignal();
}

void registerSigHandler()
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handleSignal;
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

int main(int argc, char ** argv) {
  FileSystemImpl * file_system = new FileSystemImpl();
  KernelSystemImpl * kernel_system_impl = new KernelSystemImpl();
  kernel_system_impl->set_file_system(file_system);
  AndroidSystemImpl * android_system_impl = new AndroidSystemImpl();
  android_system_impl->set_errstream(stderr);

  atrace.set_systime(new SystemTimeImpl());
  atrace.set_kernel_system(kernel_system_impl);
  atrace.set_android_system(android_system_impl);
  atrace.set_errstream(stderr);
  atrace.set_outstream(stdout);
  atrace.add_android_category( "gfx",        "Graphics",         ATRACE_TAG_GRAPHICS         );
  atrace.add_android_category( "input",      "Input",            ATRACE_TAG_INPUT            );
  atrace.add_android_category( "view",       "View System",      ATRACE_TAG_VIEW             );
  atrace.add_android_category( "webview",    "WebView",          ATRACE_TAG_WEBVIEW          );
  atrace.add_android_category( "wm",         "Window Manager",   ATRACE_TAG_WINDOW_MANAGER   );
  atrace.add_android_category( "am",         "Activity Manager", ATRACE_TAG_ACTIVITY_MANAGER );
  atrace.add_android_category( "sm",         "Sync Manager",     ATRACE_TAG_SYNC_MANAGER     );
  atrace.add_android_category( "audio",      "Audio",            ATRACE_TAG_AUDIO            );
  atrace.add_android_category( "video",      "Video",            ATRACE_TAG_VIDEO            );
  atrace.add_android_category( "camera",     "Camera",           ATRACE_TAG_CAMERA           );
  atrace.add_android_category( "hal",        "Hardware Modules", ATRACE_TAG_HAL              );
  atrace.add_android_category( "app",        "Application",      ATRACE_TAG_APP              );
  atrace.add_android_category( "res",        "Resource Loading", ATRACE_TAG_RESOURCES        );
  atrace.add_android_category( "dalvik",     "Dalvik VM",        ATRACE_TAG_DALVIK           );
  atrace.add_android_category( "rs",         "RenderScript",     ATRACE_TAG_RS               );
  atrace.add_android_category( "bionic",     "Bionic C Library", ATRACE_TAG_BIONIC           );
  atrace.add_android_category( "power",      "Power Management", ATRACE_TAG_POWER            );
  atrace.add_android_category( "pm",         "Package Manager",  ATRACE_TAG_PACKAGE_MANAGER  );
  atrace.add_android_category( "ss",         "System Server",    ATRACE_TAG_SYSTEM_SERVER    );
  atrace.add_android_category( "database",   "Database",         ATRACE_TAG_DATABASE         );
  atrace.add_android_category( "network",    "Network",          ATRACE_TAG_NETWORK          );

  atrace.set_android_core_services("core_services", "Core Services");

  atrace.add_kernel_category("sched",      "CPU Scheduling",
                         {
                            { REQ, "/sys/kernel/debug/tracing/events/sched/sched_switch/enable" },
                            { REQ, "/sys/kernel/debug/tracing/events/sched/sched_wakeup/enable" },
                            { OPT, "/sys/kernel/debug/tracing/events/sched/sched_blocked_reason/enable" },
                            { OPT, "/sys/kernel/debug/tracing/events/sched/sched_cpu_hotplug/enable" },
                         });
  atrace.add_kernel_category("irq",        "IRQ Events",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/irq/enable" },
                            { OPT,      "/sys/kernel/debug/tracing/events/ipi/enable" },
                         });
  atrace.add_kernel_category("freq",       "CPU Frequency",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/power/cpu_frequency/enable" },
                            { OPT,      "/sys/kernel/debug/tracing/events/power/clock_set_rate/enable" },
                            { OPT,      "/sys/kernel/debug/tracing/events/power/cpu_frequency_limits/enable" },
                         });
  atrace.add_kernel_category("membus",     "Memory Bus Utilization",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/memory_bus/enable" },
                         });
  atrace.add_kernel_category("idle",       "CPU Idle",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/power/cpu_idle/enable" },
                         });
  atrace.add_kernel_category("disk",       "Disk I/O",
                         {
                            { OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_sync_file_enter/enable" },
                            { OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_sync_file_exit/enable" },
                            { OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_write_begin/enable" },
                            { OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_write_end/enable" },
                            { OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_da_write_begin/enable" },
                            { OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_da_write_end/enable" },
                            { OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_sync_file_enter/enable" },
                            { OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_sync_file_exit/enable" },
                            { REQ,      "/sys/kernel/debug/tracing/events/block/block_rq_issue/enable" },
                            { REQ,      "/sys/kernel/debug/tracing/events/block/block_rq_complete/enable" },
                         });
  atrace.add_kernel_category("mmc",        "eMMC commands",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/mmc/enable" },
                         });
  atrace.add_kernel_category("load",       "CPU Load",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/cpufreq_interactive/enable" },
                         });
  atrace.add_kernel_category("sync",       "Synchronization",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/sync/enable" },
                         });
  atrace.add_kernel_category("workq",      "Kernel Workqueues",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/workqueue/enable" },
                         });
  atrace.add_kernel_category("memreclaim", "Kernel Memory Reclaim",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_begin/enable" },
                            { REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_end/enable" },
                            { REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_wake/enable" },
                            { REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_sleep/enable" },
                         });
  atrace.add_kernel_category("regulators",  "Voltage and Current Regulators",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/regulator/enable" },
                         });
  atrace.add_kernel_category("binder_driver", "Binder Kernel driver",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction/enable" },
                            { REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction_received/enable" },
                         });
  atrace.add_kernel_category("binder_lock", "Binder global lock trace",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/binder/binder_lock/enable" },
                            { REQ,      "/sys/kernel/debug/tracing/events/binder/binder_locked/enable" },
                            { REQ,      "/sys/kernel/debug/tracing/events/binder/binder_unlock/enable" },
                         });
  atrace.add_kernel_category("pagecache",  "Page cache",
                         {
                            { REQ,      "/sys/kernel/debug/tracing/events/filemap/enable" },
                         });
  registerSigHandler();
  int res = atrace.run_atrace(argc, argv);
  return res;
}
