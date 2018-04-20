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

#include "atraceapp.h"

#include <unistd.h> // sleep
#include <errno.h>
#include <string.h>

AtraceApp::~AtraceApp() {
  delete kernel_system;
  delete android_system;
}

void AtraceApp::handleSignal()
{
    if (!g_nohup) {
        g_traceAborted = true;
    }
}

void AtraceApp::set_android_system(AndroidSystem * android_system) {
    this->android_system = android_system;
}

void AtraceApp::set_kernel_system(KernelSystem * kernel_system) {
    this->kernel_system = kernel_system;
}

void AtraceApp::set_errstream(FILE * errstream) {
    this->errstream = errstream;
}

void AtraceApp::set_outstream(FILE * outstream) {
    this->outstream = outstream;
}

void AtraceApp::set_toolbox(Toolbox * toolbox)
{
    this->toolbox = toolbox;
}

void AtraceApp::set_debugAppCmdLine(const char * app) 
{
    g_debugAppCmdLine = app;
}

void AtraceApp::set_traceBufferSizeKB(int size) 
{
    g_traceBufferSizeKB = size;
}

void AtraceApp::enable_trace_overwrite() 
{
    g_traceOverwrite = true;
}

void AtraceApp::set_kernelTraceFuncs(const char * funcs) 
{
    g_kernelTraceFuncs = funcs;
}

void AtraceApp::nosignals() 
{
    g_nohup = true;
}

void AtraceApp::set_initialSleepSecs(int secs) 
{
    g_initialSleepSecs = secs;
}

void AtraceApp::set_traceDurationSeconds(int secs) 
{
    g_traceDurationSeconds = secs;
}

void AtraceApp::enable_compression() 
{
    g_compress = true;
}

void AtraceApp::set_outputFile(const char * filename) 
{
    g_outputFile = filename;
}

void AtraceApp::set_async(bool option) 
{
    async = option;
}

void AtraceApp::set_start(bool option) 
{
    traceStart = option;
}

void AtraceApp::set_stop(bool option) 
{
    traceStop = option;
}

void AtraceApp::set_dump(bool option) 
{
    traceDump = option;
}

void AtraceApp::enable_streaming() 
{
    traceStream = true;
}

bool AtraceApp::setCategory(const char* name)
{
    for (size_t i = 0; i < k_categories.size(); i++) {
        TracingCategory& c = k_categories[i];
        if (strcmp(name, c.name) == 0) {
            if (isCategorySupported(c)) {
                c.is_enabled = true;
                return true;
            }
            return false;
        }
    }
    fprintf(errstream, "error: unknown tracing category \"%s\"\n", name);
    return false;
}

void AtraceApp::add_android_category(const char * id)
{
  androidCategories.push_back(id);
}

void AtraceApp::add_kernel_category(const char * id, const char * name, const std::vector<EnableFile> &files)
{
  k_categories.push_back({id, name, 0, files, false });
}

bool AtraceApp::run()
{
    if (g_initialSleepSecs > 0) {
        sleep(g_initialSleepSecs);
    }

    bool ok = true;
    ok &= setUpTrace();
    ok &= startTrace();

    if (ok && traceStart) {
        if (!traceStream) {
            printf("capturing trace...");
            fflush(outstream);
        }

        // We clear the trace after starting it because tracing gets enabled for
        // each CPU individually in the kernel. Having the beginning of the trace
        // contain entries from only one CPU can cause "begin" entries without a
        // matching "end" entry to show up if a task gets migrated from one CPU to
        // another.
        ok = kernel_system->clearTrace();

        kernel_system->writeClockSyncMarker();
        if (ok && !async && !traceStream) {
            // Sleep to allow the trace to be captured.
            struct timespec timeLeft;
            timeLeft.tv_sec = g_traceDurationSeconds;
            timeLeft.tv_nsec = 0;
            do {
                if (g_traceAborted) {
                    break;
                }
            } while (nanosleep(&timeLeft, &timeLeft) == -1 && errno == EINTR);
        }

        if (traceStream) {
            streamTrace();
        }
    }

    // Stop the trace and restore the default settings.
    if (traceStop)
        stopTrace();
    if (ok && traceDump) {
        if (!g_traceAborted) {
            printf(" done\n");
            fflush(outstream);
            dumpTrace();       
        } else {
            printf("\ntrace aborted.\n");
            fflush(outstream);
        }
        kernel_system->clearTrace();
    } else if (!ok) {
        fprintf(errstream, "unable to start tracing\n");
    }

    // Reset the trace buffer size to 1.
    if (traceStop)
        cleanUpTrace();

    return !g_traceAborted;
}

// Check whether the category is supported on the device with the current
// rootness.  A category is supported only if all its required /sys/ files are
// writable and if enabling the category will enable one or more tracing tags
// or /sys/ files.
bool AtraceApp::isCategorySupported(const TracingCategory& category)
{
    // if (strcmp(category.name, k_coreServiceCategory) == 0) {
    //     return android_system->has_core_services();
    // }

    bool ok = true;
    if (category.tags == 0) {
        ok = kernel_system->isCategorySupported(category);
    }
    return ok;
}


// Disable all /sys/ enable files.
bool AtraceApp::disableKernelTraceEvents() {
    bool ok = true;
    for (size_t i = 0; i < k_categories.size(); i++) {
        const TracingCategory &c = k_categories[i];
        for (const auto & file : c.files) {
            const char* path = file.path;
            if (kernel_system->isPossibleSetKernelOption(path)) {
                ok &= kernel_system->setKernelOptionEnable(path, false);
            }
        }
    }
    return ok;
}

// Set all the kernel tracing settings to the desired state for this trace
// capture.
bool AtraceApp::setUpTrace()
{
    bool ok = true;

    // Set up the tracing options.
    ok &= kernel_system->setTraceOverwriteEnable(g_traceOverwrite);
    ok &= kernel_system->setTraceBufferSizeKB(g_traceBufferSizeKB);
    ok &= kernel_system->setGlobalClockEnable(true);
    ok &= kernel_system->setPrintTgidEnableIfPresent(true);
    ok &= kernel_system->setKernelTraceFuncs(g_kernelTraceFuncs.c_str());

    // Set up the tags property.
    ok &= android_system->tryEnableCategories(androidCategories);
    
    std::string packageList(g_debugAppCmdLine);
    if (enableCoreServices) {
        if (android_system->has_core_services()) {
            std::string value;
            android_system->property_get_core_service_names(value);
            if (!packageList.empty()) {
                packageList += ",";
            }
            packageList += value;
        } else {
            fprintf(errstream, "Can't enable core services - not supported\n");
        }
    }
    ok &= android_system->setAppCmdlineProperty(packageList.data());
    ok &= android_system->pokeBinderServices();

    // Disable all the sysfs enables.  This is done as a separate loop from
    // the enables to allow the same enable to exist in multiple categories.
    ok &= disableKernelTraceEvents();

    // Enable all the sysfs enables that are in an enabled category.
    for (size_t i = 0; i < k_categories.size(); i++) {
        const TracingCategory &c = k_categories[i];
        if (c.is_enabled) {
            for (const auto & file : c.files) {
                // const char* path = enable_file_paths[c.files[j].file_id];
                const char* path = file.path;
                bool required = file.required == EnableFile::REQ;
                if (path != NULL) {
                    if (kernel_system->isPossibleSetKernelOption(path)) {
                        ok &= kernel_system->setKernelOptionEnable(path, true);
                    } else if (required) {
                        fprintf(errstream, "error writing file %s\n", path);
                        ok = false;
                    }
                }
            }
        }
    }

    return ok;
}

// Reset all the kernel tracing settings to their default state.
void AtraceApp::cleanUpTrace()
{
    // Disable all tracing that we're able to.
    disableKernelTraceEvents();

    // Reset the system properties.
    android_system->disableAllCategories();
    android_system->clearAppProperties();
    android_system->pokeBinderServices();

    // Set the options back to their defaults.
    kernel_system->setTraceOverwriteEnable(true);
    kernel_system->setTraceBufferSizeKB(1);
    kernel_system->setGlobalClockEnable(false);
    kernel_system->setPrintTgidEnableIfPresent(false);
    kernel_system->setKernelTraceFuncs(NULL);
}


// Enable tracing in the kernel.
bool AtraceApp::startTrace()
{
    return kernel_system->setTracingEnabled(true);
}

// Disable tracing in the kernel.
void AtraceApp::stopTrace()
{
    kernel_system->setTracingEnabled(false);
}

// Read data from the tracing pipe and forward to outstream
void AtraceApp::streamTrace()
{
    int trace_stream = kernel_system->getTracePipeFd();
    if (trace_stream == -1) {
        fprintf(errstream, "error streaming trace\n");
        return;
    }
    while (!g_traceAborted) {
        if (!kernel_system->try_send(trace_stream, fileno(outstream))) {
            if (!g_traceAborted) {
              fprintf(errstream, "error streaming trace");
            }
            break;
        }
        fflush(outstream);
    }
}

// Read the current kernel trace and write it to outstream.
void AtraceApp::dumpTrace()
{
    int outFd = fileno(outstream);
    if (!g_outputFile.empty()) {
      outFd = kernel_system->tryOpenToWriteOrCreate(g_outputFile.c_str());
      if (outFd == -1) {
        fprintf(errstream, "error dumping trace\n");
        return;
      }
    } 
    dprintf(outFd, "TRACE:\n");
    android_system->log_dumping_trace();

    int traceFD = kernel_system->getTraceFd();
    if (traceFD == -1) {
        fprintf(errstream, "error dumping trace\n");
        return;
    }
    if (g_compress) {
        android_system->compress_trace_to(traceFD, outFd);
    } else {
        // ssize_t sent = 0;
        // while ((sent = sendfile(outFd, traceFD, NULL, 64*1024*1024)) > 0);
        if (!kernel_system->try_sendfile(traceFD, outFd)) {
            fprintf(errstream, "error dumping trace: %s (%d)\n", strerror(errno),
                    errno);
        }
    }
    close(traceFD);
    if (!g_outputFile.empty()) {
        close(outFd);
    }
}

void AtraceApp::enableAndroidCore() {
    enableCoreServices = true;
}

void AtraceApp::listSupportedCategories()
{
    for (size_t i = 0; i < k_categories.size(); i++) {
        const TracingCategory& c = k_categories[i];
        if (isCategorySupported(c)) {
            printf("  %10s - %s\n", c.name, c.longname);
        }
    }
    const auto & androidCategories = android_system->getCategories();
    for (const auto & category : androidCategories) {
        printf("  %10s - %s\n", category.name, category.longname);
    }
}
