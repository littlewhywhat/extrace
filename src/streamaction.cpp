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

bool StreamAction::tryRunIn(Environment & environment, TraceSystem & traceSystem) {
  bool ok = true;
  int traceStream = traceSystem.getKernelSystem().getTracePipeFd();
  if (traceStream == -1) {
      fprintf(m_Wire.getErrorStream(), "error StreamAction::tryRun\n");
      return false;
  }
  FILE * outputStream = m_Wire.getOutputStream();
  while (!environment.isInterrupted()) {
      if (!traceSystem.getKernelSystem().try_send(traceStream, fileno(outputStream))) {
          if (!environment.isInterrupted()) {
            fprintf(m_Wire.getErrorStream(), "error StreamAction::tryRun - stream aborted\n");
            ok = false;
          }
          break;
      }
      fflush(outputStream);
  }
  return ok;
}
