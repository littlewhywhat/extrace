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
#include "streamaction.h"

bool StreamAction::tryRun() {
  bool ok = true;
  int traceStream = m_TraceSystem->getKernelSystem().getTracePipeFd();
  if (traceStream == -1) {
      fprintf(m_Wire.getErrorStream(), "error StreamAction::tryRun\n");
      return false;
  }
  FILE * outputStream = m_Wire.getOutputStream();
  while (!m_Signal.isFired()) {
      if (!m_TraceSystem->getKernelSystem().try_send(traceStream, fileno(outputStream))) {
          if (!m_Signal.isFired()) {
            fprintf(m_Wire.getErrorStream(), "error StreamAction::tryRun - stream aborted\n");
            ok = false;
          }
          break;
      }
      fflush(outputStream);
  }
  return ok;
}
