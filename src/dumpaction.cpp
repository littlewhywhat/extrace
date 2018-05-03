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

void DumpAction::setOutputFile(const string & outputFile) {
  m_OutputFile = outputFile;
}

void DumpAction::enableCompression() {
  m_Compress = true;
}

bool DumpAction::tryRun() {
  FILE * errorStream = m_Wire.getErrorStream();
  auto & kernelSystem = m_TraceSystem->getKernelSystem();
  auto & fileSystem = m_TraceSystem->getFileSystem();

  int outFd;
  if (!m_OutputFile.empty()) {
    outFd = fileSystem.tryOpenFileToWriteOrCreate(m_OutputFile.c_str());
    if (outFd == -1) {
      fprintf(errorStream, "error DumpAction::tryRun\n");
      return false;
    }
  }
  else {
    outFd = fileno(m_Wire.getOutputStream());
  }

  bool ok = true;
  if (m_Compress) {
    ok &= kernelSystem.trySendTraceCompressedTo(outFd);
  } else {
    ok &= kernelSystem.trySendTraceTo(outFd);
  }
  if (!ok) {
    fprintf(errorStream, "error DumpAction::tryRun\n");
  }

  if (!m_OutputFile.empty()) {
    close(outFd);
  }
  return ok;
}

TraceAction * DumpAction::Builder::buildFrom(const Wire & wire,
                                             const shared_ptr<TraceSystem> & traceSystem,
                                             const ExtraceArguments & arguments) const {
  auto * dumpAction = new DumpAction(wire, traceSystem);
  if (arguments.compressionEnabled())
  {
    dumpAction->enableCompression();
  }
  if (arguments.hasOutputFilename())
  {
    dumpAction->setOutputFile(arguments.getOutputFilename());
  }
  return dumpAction;
}