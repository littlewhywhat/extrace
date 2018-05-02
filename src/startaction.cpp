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

bool StartAction::tryRun() {
  bool ok = true;
  ok &= m_TraceSystem->getTrace().setUp();
  ok &= m_TraceSystem->getTrace().start();

  if (ok) {
    fprintf(m_Wire.getOutputStream(), "started trace...\n");
    fflush(m_Wire.getOutputStream());

    // We clear the trace after starting it because tracing gets enabled for
    // each CPU individually in the kernel. Having the beginning of the trace
    // contain entries from only one CPU can cause "begin" entries without a
    // matching "end" entry to show up if a task gets migrated from one CPU to
    // another.
    ok &= m_TraceSystem->getKernelSystem().clearTrace();
    ok &= m_TraceSystem->getKernelSystem().writeClockSyncMarker();
  } else {
    fprintf(m_Wire.getErrorStream(), "error StartAction::tryRun\n");
  }
  return ok;
}
