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

bool DumpAction::tryDumpToOutput() {
  auto & traceBuffer = m_Environment->getTraceBuffer();
  if (!traceBuffer.trySendTo(fileno(m_Wire.getOutputStream()))) {
    fprintf(m_Wire.getErrorStream(), "Can't send to output\n");
    return false;
  }
  return true;
}

bool DumpAction::tryDumpToFile() {
  auto & traceBuffer = m_Environment->getTraceBuffer();
  auto & fileSystem  = m_Environment->getFileSystem();

  int outFd = fileSystem.tryOpenFileToWriteOrCreate(m_OutputFile.c_str());
  if (outFd == -1) {
    fprintf(m_Wire.getErrorStream(), "error DumpAction::tryRun\n");
    return false;
  }
  bool ok = traceBuffer.trySendTo(outFd);
  close(outFd);
  return ok;
}

bool DumpAction::tryRun() {
  
  if (!m_OutputFile.empty()) {
    return tryDumpToFile();
  } 
  return tryDumpToOutput();
}

DumpAction * DumpAction::Builder::buildFrom(const Wire & wire,
                                             const shared_ptr<Environment> & environment,
                                             const ExtraceArguments & arguments) const {
  auto * dumpAction = new DumpAction(wire, environment);
  if (arguments.hasOutputFilename())
  {
    dumpAction->setOutputFile(arguments.getOutputFilename());
  }
  return dumpAction;
}