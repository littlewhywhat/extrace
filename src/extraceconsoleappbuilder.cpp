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
#include "extraceconsoleapp.h"
#include "extraceconsoleappbuilder.h"


ExtraceConsoleApp * ExtraceConsoleAppBuilder::build(void) const
{
  // TODO
  // FMachineTrace * fMachineTrace = new FMachineTrace();
  // MachineTraceSession * machineTraceSession = new FMachineTraceSession(fMachineTrace);
  // ExtraceApp * extraceApp = new ExtraceAppImpl(machineTraceSession);

  ActionRunnerBuilder * actionRunnerBuilder = new ActionRunnerBuilder();
  actionRunnerBuilder->setSystemCoreBuilder(new SystemCore::Builder());

  ExtraceConsoleApp * consoleApp = new ExtraceConsoleApp();
  consoleApp->setErrorStream(stderr);
  consoleApp->setActionRunnerBuilder(actionRunnerBuilder);
  return consoleApp;
}
