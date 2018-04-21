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
#include "dumpaction.h"

#include <unistd.h> // close

void DumpAction::setKernelSystem(shared_ptr<KernelSystem> & kernelSystem) {
  m_KernelSystem = kernelSystem;
}

void DumpAction::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

void DumpAction::setOutputFile(const char * outputFile) {
  m_OutputFile = outputFile;
}

void DumpAction::setOutputStream(FILE * outStream) {
  m_OutStream = outStream;
}

void DumpAction::enableCompression() {
  m_Compress = true;
}

bool DumpAction::tryRun() {
  int outFd = fileno(m_OutStream);
  if (!m_OutputFile.empty()) {
    outFd = m_KernelSystem->tryOpenToWriteOrCreate(m_OutputFile.c_str());
    if (outFd == -1) {
      fprintf(m_ErrorStream, "error DumpAction::tryRun\n");
      return false;
    }
  } 
  dprintf(outFd, "TRACE:\n");

  int traceFD = m_KernelSystem->getTraceFd();
  if (traceFD == -1) {
    fprintf(m_ErrorStream, "error DumpAction::tryRun\n");
    return false;
  }
  bool ok = true;
  if (m_Compress) {
    ok &= m_KernelSystem->compress_trace_to(traceFD, outFd);
  } else {
    ok &= m_KernelSystem->try_sendfile(traceFD, outFd);
  }
  if (!ok) {
    fprintf(m_ErrorStream, "error DumpAction::tryRun\n");
  }
  close(traceFD);
  if (!m_OutputFile.empty()) {
    close(outFd);
  }
  return m_KernelSystem->clearTrace();
}