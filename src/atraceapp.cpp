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

#include <getopt.h> // getopt_long, no_argument
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h> // sleep
#include <errno.h>
#include <sys/sendfile.h>
#include <fcntl.h>  // O_WRONLY, O_CREAT

AtraceApp::~AtraceApp() {
  delete systime;
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

void AtraceApp::set_systime(SystemTime * systime) {
    this->systime = systime;
}

void AtraceApp::set_errstream(FILE * errstream) {
    this->errstream = errstream;
}

void AtraceApp::set_outstream(FILE * outstream) {
    this->outstream = outstream;
}

void AtraceApp::add_android_category(const char * id, const char * name, uint64_t atrace_tag)
{
  k_categories.push_back({ id, name, atrace_tag, {}, false });
}

void AtraceApp::add_kernel_category(const char * id, const char * name, const std::vector<EnableFile> &files)
{
  k_categories.push_back({id, name, 0, files, false });
}

void AtraceApp::set_android_core_services(const char * id, const char * name)
{
  k_coreServiceCategory = id;
  k_categories.push_back({ k_coreServiceCategory, name, 0, { }, false });
}

int AtraceApp::run_atrace(int argc, char **argv)
{
    bool async = false;
    bool traceStart = true;
    bool traceStop = true;
    bool traceDump = true;
    bool traceStream = false;

    if (argc == 2 && 0 == strcmp(argv[1], "--help")) {
        showHelp(argv[0]);
        return EXIT_SUCCESS;
    }

    for (;;) {
        int ret;
        int option_index = 0;
        static struct option long_options[] = {
            {"async_start",     no_argument, 0,  0 },
            {"async_stop",      no_argument, 0,  0 },
            {"async_dump",      no_argument, 0,  0 },
            {"list_categories", no_argument, 0,  0 },
            {"stream",          no_argument, 0,  0 },
            {           0,                0, 0,  0 }
        };

        ret = getopt_long(argc, argv, "a:b:cf:k:ns:t:zo:",
                          long_options, &option_index);

        if (ret < 0) {
            for (int i = optind; i < argc; i++) {
                if (!setCategoryEnable(argv[i], true)) {
                    fprintf(errstream, "error enabling tracing category \"%s\"\n", argv[i]);
                    return EXIT_FAILURE;
                }
            }
            break;
        }

        switch(ret) {
            case 'a':
                g_debugAppCmdLine = optarg;
            break;

            case 'b':
                g_traceBufferSizeKB = atoi(optarg);
            break;

            case 'c':
                g_traceOverwrite = true;
            break;

            case 'f':
                g_categoriesFile = optarg;
            break;

            case 'k':
                g_kernelTraceFuncs = optarg;
            break;

            case 'n':
                g_nohup = true;
            break;

            case 's':
                g_initialSleepSecs = atoi(optarg);
            break;

            case 't':
                g_traceDurationSeconds = atoi(optarg);
            break;

            case 'z':
                g_compress = true;
            break;

            case 'o':
                g_outputFile = optarg;
            break;

            case 0:
                if (!strcmp(long_options[option_index].name, "async_start")) {
                    async = true;
                    traceStop = false;
                    traceDump = false;
                    g_traceOverwrite = true;
                } else if (!strcmp(long_options[option_index].name, "async_stop")) {
                    async = true;
                    traceStart = false;
                } else if (!strcmp(long_options[option_index].name, "async_dump")) {
                    async = true;
                    traceStart = false;
                    traceStop = false;
                } else if (!strcmp(long_options[option_index].name, "stream")) {
                    traceStream = true;
                    traceDump = false;
                } else if (!strcmp(long_options[option_index].name, "list_categories")) {
                    listSupportedCategories();
                    return EXIT_SUCCESS;
                }
            break;

            default:
                fprintf(errstream, "\n");
                showHelp(argv[0]);
                return EXIT_FAILURE;
            break;
        }
    }

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

        writeClockSyncMarker();
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
            int outFd = fileno(outstream);
            if (g_outputFile) {
                outFd = open(g_outputFile, O_WRONLY | O_CREAT);
            }
            if (outFd == -1) {
                printf("Failed to open '%s', err=%d", g_outputFile, errno);
            } else {
                dprintf(outFd, "TRACE:\n");
                dumpTrace(outFd);
                if (g_outputFile) {
                    close(outFd);
                }
            }
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

    return g_traceAborted ? EXIT_FAILURE : EXIT_SUCCESS;
}

bool AtraceApp::writeClockSyncMarker()
{
  char buffer[128];
  float now_in_seconds = systime->get_monotonic();
  snprintf(buffer, 128, "trace_event_clock_sync: parent_ts=%f\n", now_in_seconds);
  bool ok = true;
  ok &= kernel_system->writeMarker(buffer);
  // ok &= writeStr(k_traceMarkerPath, buffer);
  int64_t realtime_in_ms = systime->get_realtime();
  snprintf(buffer, 128, "trace_event_clock_sync: realtime_ts=%" PRId64 "\n", realtime_in_ms);
  ok &= kernel_system->writeMarker(buffer);
  // ok &= writeStr(k_traceMarkerPath, buffer);
  return ok;
}

// Check whether the category is supported on the device with the current
// rootness.  A category is supported only if all its required /sys/ files are
// writable and if enabling the category will enable one or more tracing tags
// or /sys/ files.
bool AtraceApp::isCategorySupported(const TracingCategory& category)
{
    if (strcmp(category.name, k_coreServiceCategory) == 0) {
        return android_system->has_core_services();
    }

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

bool AtraceApp::setCategoryEnable(const char* name, bool enable)
{
    for (size_t i = 0; i < k_categories.size(); i++) {
        TracingCategory& c = k_categories[i];
        if (strcmp(name, c.name) == 0) {
            if (isCategorySupported(c)) {
                c.is_enabled = enable;
                return true;
            } else {
                if (kernel_system->isCategorySupportedForRoot(c)) {
                    fprintf(errstream, "error: category \"%s\" requires root "
                            "privileges.\n", name);
                } else {
                    fprintf(errstream, "error: category \"%s\" is not supported "
                            "on this device.\n", name);
                }
                return false;
            }
        }
    }
    fprintf(errstream, "error: unknown tracing category \"%s\"\n", name);
    return false;
}

// Set all the kernel tracing settings to the desired state for this trace
// capture.
bool AtraceApp::setUpTrace()
{
    bool ok = true;

    // Set up the tracing options.
    // ok &= android_system->setCategoriesEnableFromFile(g_categoriesFile);
    ok &= kernel_system->setTraceOverwriteEnable(g_traceOverwrite);
    ok &= kernel_system->setTraceBufferSizeKB(g_traceBufferSizeKB);
    ok &= kernel_system->setGlobalClockEnable(true);
    ok &= kernel_system->setPrintTgidEnableIfPresent(true);
    ok &= kernel_system->setKernelTraceFuncs(g_kernelTraceFuncs);

    // Set up the tags property.
    uint64_t tags = 0;
    for (size_t i = 0; i < k_categories.size(); i++) {
        TracingCategory &c = k_categories[i];
        if (c.is_enabled) {
            tags |= c.tags;
        }
    }
    ok &= android_system->setTagsProperty(tags);

    bool coreServicesTagEnabled = false;
    for (size_t i = 0; i < k_categories.size(); i++) {
        const TracingCategory &c = k_categories[i];
        if (strcmp(c.name, k_coreServiceCategory) == 0) {
            coreServicesTagEnabled = c.is_enabled;
        }
    }

    std::string packageList(g_debugAppCmdLine);
    if (coreServicesTagEnabled) {
        std::string value;
        android_system->property_get_core_service_names(value);
        if (!packageList.empty()) {
            packageList += ",";
        }
        packageList += value;
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
                        fprintf(stderr, "error writing file %s\n", path);
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
    android_system->setTagsProperty(0);
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
    char trace_data[4096];
    int traceFD = kernel_system->getTracePipeFd();
    if (traceFD == -1) {
        fprintf(errstream, "error opening trace stream\n");
        return;
    }
    while (!g_traceAborted) {
        ssize_t bytes_read = read(traceFD, trace_data, 4096);
        if (bytes_read > 0) {
            write(fileno(outstream), trace_data, bytes_read);
            fflush(outstream);
        } else {
            if (!g_traceAborted) {
                fprintf(errstream, "read returned %zd bytes err %d (%s)\n",
                        bytes_read, errno, strerror(errno));
            }
            break;
        }
    }
}

// Read the current kernel trace and write it to outstream.
void AtraceApp::dumpTrace(int outFd)
{
    android_system->log_dumping_trace();
    int traceFD = kernel_system->getTraceFd();
    if (traceFD == -1) {
        fprintf(errstream, "error opening trace\n");
        return;
    }

    if (g_compress) {
        android_system->compress_trace_to(traceFD, outFd);
    } else {
        ssize_t sent = 0;
        while ((sent = sendfile(outFd, traceFD, NULL, 64*1024*1024)) > 0);
        if (sent == -1) {
            fprintf(errstream, "error dumping trace: %s (%d)\n", strerror(errno),
                    errno);
        }
    }

    close(traceFD);
}


void AtraceApp::listSupportedCategories()
{
    for (size_t i = 0; i < k_categories.size(); i++) {
        const TracingCategory& c = k_categories[i];
        if (isCategorySupported(c)) {
            printf("  %10s - %s\n", c.name, c.longname);
        }
    }
}

// Print the command usage help to errstream.
void AtraceApp::showHelp(const char *cmd)
{
    fprintf(errstream, "usage: %s [options] [categories...]\n", cmd);
    fprintf(errstream, "options include:\n"
                    "  -a appname      enable app-level tracing for a comma "
                        "separated list of cmdlines\n"
                    "  -b N            use a trace buffer size of N KB\n"
                    "  -c              trace into a circular buffer\n"
                    "  -f filename     use the categories written in a file as space-separated\n"
                    "                    values in a line\n"
                    "  -k fname,...    trace the listed kernel functions\n"
                    "  -n              ignore signals\n"
                    "  -s N            sleep for N seconds before tracing [default 0]\n"
                    "  -t N            trace for N seconds [defualt 5]\n"
                    "  -z              compress the trace dump\n"
                    "  --async_start   start circular trace and return immediatly\n"
                    "  --async_dump    dump the current contents of circular trace buffer\n"
                    "  --async_stop    stop tracing and dump the current contents of circular\n"
                    "                    trace buffer\n"
                    "  --stream        stream trace to outstream as it enters the trace buffer\n"
                    "                    Note: this can take significant CPU time, and is best\n"
                    "                    used for measuring things that are not affected by\n"
                    "                    CPU performance, like pagecache usage.\n"
                    "  --list_categories\n"
                    "                  list the available tracing categories\n"
                    " -o filename      write the trace to the specified file instead\n"
                    "                    of outstream.\n"
            );
}
