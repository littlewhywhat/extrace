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
  if (!m_OutputFile.empty()) {
    if (m_Compress) {
      return m_Trace->trySendCompressedTo(m_OutputFile);
    } 
    return m_Trace->trySendTo(m_OutputFile);
  } 
  else if (m_Compress) {
    return m_Trace->trySendCompressedToOutput();
  }
  else {
    return m_Trace->trySendToOutput();
  }
}

TraceAction * DumpAction::Builder::buildFrom(const Wire & wire,
                                             const shared_ptr<Trace> & trace,
                                             const ExtraceArguments & arguments) const {
  auto * dumpAction = new DumpAction(wire, trace);
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