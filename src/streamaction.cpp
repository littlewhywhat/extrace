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

void StreamAction::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

void StreamAction::setOutputStream(FILE * outStream) {
  m_OutStream = outStream;
}

void StreamAction::setKernelSystem(KernelSystem * kernelSystem) {
  m_KernelSystem = kernelSystem;
}

bool StreamAction::tryRun() {
  bool ok = true;
  int traceStream = m_KernelSystem->getTracePipeFd();
  if (traceStream == -1) {
      fprintf(m_ErrorStream, "error StreamAction::tryRun\n");
      return false;
  }
  while (!m_Interrupted) {
      if (!m_KernelSystem->try_send(traceStream, fileno(m_OutStream))) {
          if (!m_Interrupted) {
            fprintf(m_ErrorStream, "error StreamAction::tryRun - stream aborted\n");
            ok = false;
          }
          break;
      }
      fflush(m_OutStream);
  }
  return ok;
}

InterruptableAction * StreamAction::Builder::buildFrom(const SystemCore & systemCore) const {
  StreamAction * streamAction = new StreamAction();
  streamAction->setErrorStream(systemCore.getErrorStream());
  streamAction->setOutputStream(systemCore.getOutputStream());
  streamAction->setKernelSystem(systemCore.getKernelSystem());
  return streamAction;
}