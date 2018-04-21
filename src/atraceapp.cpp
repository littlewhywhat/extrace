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
  delete m_Trace;
}

void AtraceApp::handleSignal()
{
    if (!g_nohup) {
        g_traceAborted = true;
    }
}

void AtraceApp::setTrace(Trace * trace) {
  m_Trace = trace;
}

void AtraceApp::set_android_system(shared_ptr<AndroidSystem> & android_system) {
    this->android_system = android_system;
}

void AtraceApp::set_kernel_system(shared_ptr<KernelSystem> & kernel_system) {
    this->kernel_system = kernel_system;
}

void AtraceApp::set_errstream(FILE * errstream) {
    this->errstream = errstream;
}

void AtraceApp::set_outstream(FILE * outstream) {
    this->outstream = outstream;
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

bool AtraceApp::run()
{
    if (g_initialSleepSecs > 0) {
        sleep(g_initialSleepSecs);
    }

    bool ok = true;
    ok &= m_Trace->setUp();
    ok &= m_Trace->start();

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
        m_Trace->stop();
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
        m_Trace->cleanUp();

    return !g_traceAborted;
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
        kernel_system->compress_trace_to(traceFD, outFd);
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
