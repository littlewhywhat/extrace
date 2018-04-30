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
#include <signal.h> // sigaction

#include "extraceconsoleappbuilder.h"
#include "consoleappmain.h"

ConsoleAppMain consoleAppMain;

void handleSignal(int /*signo*/)
{
  consoleAppMain.handleSignal();
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

//! Builds ExtraceConsoleApp and runs it with ConsoleAppMain
int main(int argc, const char ** argv)
{
  consoleAppMain.set_consoleapp(ExtraceConsoleAppBuilder().build());
  registerSigHandler();
  return consoleAppMain.run(argc, argv);
}
