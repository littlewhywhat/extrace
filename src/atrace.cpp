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

#include "filesystem_impl.h"
#include "systemtime_impl.h"
#include "androidsystem_impl.h"
#include "kernelsystem_impl.h"
#include "androidtoolbox.h"
#include "trace_impl.h"
#include "signal.h"
#include "startaction.h"
#include "stopaction.h"
#include "sleepaction.h"
#include "dumpaction.h"
#include "streamaction.h"
#include "cleanupaction.h"
#include "actionrunner_impl.h"
#include "extraceapp.h"

#include <signal.h>      // sigaction
#include <stdlib.h>      // EXIT_FAILURE, EXIT_SUCCESS

using namespace std;

ExtraceApp extraceApp;

void handleSignal(int /*signo*/)
{
  extraceApp.handleSignal();
}

void setupExtraceApp() {
  extraceApp.setErrorStream(stderr);
  extraceApp.setOutputStream(stdout);
  extraceApp.setAndroidToolbox(new AndroidToolbox());
  extraceApp.setSystemTimeImpl(new SystemTimeImpl());
  extraceApp.setFileSystemImpl(new FileSystemImpl());
  extraceApp.setKernelSystemImpl(new KernelSystemImpl());
  extraceApp.setAndroidSystemImpl(new AndroidSystemImpl());
  extraceApp.setTraceImpl(new TraceImpl());
  extraceApp.setInitSleepAction(new SleepAction());
  extraceApp.setStartAction(new StartAction());
  extraceApp.setMidSleepAction(new SleepAction());
  extraceApp.setStreamAction(new StreamAction());
  extraceApp.setStopAction(new StopAction());
  extraceApp.setDumpAction(new DumpAction());
  extraceApp.setCleanUpAction(new CleanUpAction());
  extraceApp.setSignal(new Signal());
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

int main(int argc, const char ** argv) {
  registerSigHandler();
  setupExtraceApp();

  if (extraceApp.run(argc, argv))
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}
