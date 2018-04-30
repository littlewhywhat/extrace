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
#include "startaction.h"

void StartAction::setTrace(Trace * trace) {
  m_Trace = trace;
}

void StartAction::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

void StartAction::setOutputStream(FILE * outStream) {
  m_OutStream = outStream;
}

void StartAction::setKernelSystem(KernelSystem * kernelSystem) {
  m_KernelSystem = kernelSystem;
}

bool StartAction::tryRun() {
  bool ok = true;
  ok &= m_Trace->setUp();
  ok &= m_Trace->start();

  if (ok) {
    fprintf(m_OutStream, "started trace...\n");
    fflush(m_OutStream);

    // We clear the trace after starting it because tracing gets enabled for
    // each CPU individually in the kernel. Having the beginning of the trace
    // contain entries from only one CPU can cause "begin" entries without a
    // matching "end" entry to show up if a task gets migrated from one CPU to
    // another.
    ok &= m_KernelSystem->clearTrace();
    ok &= m_KernelSystem->writeClockSyncMarker();
  } else {
    fprintf(m_ErrorStream, "error StartAction::tryRun\n");
  }
  return ok;
}

Action * StartAction::Builder::buildFrom(const SystemCore & systemCore) const
{
  auto * startAction = new StartAction();
  startAction->setTrace(systemCore.getTrace());
  startAction->setErrorStream(systemCore.getErrorStream());
  startAction->setOutputStream(systemCore.getOutputStream());
  startAction->setKernelSystem(systemCore.getKernelSystem());
  return startAction;
}