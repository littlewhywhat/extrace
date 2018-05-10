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

#include "environment.h"

Environment::Environment(const shared_ptr<Trace> & trace,
                         const shared_ptr<KernelTraceSystem> & kernelTraceSystem,
                         const shared_ptr<AndroidTraceSystem> & androidTraceSystem, 
                         const shared_ptr<TraceBuffer> & traceBuffer,
                         const shared_ptr<FileSystem> & fileSystem,
                         const shared_ptr<ToolBox> & toolBox):
                         m_Trace(trace),
                         m_KernelTraceSystem(kernelTraceSystem),
                         m_AndroidTraceSystem(androidTraceSystem),
                         m_TraceBuffer(traceBuffer),
                         m_FileSystem(fileSystem),
                         m_ToolBox(toolBox) {} 

Trace & Environment::getTrace() {
  return *m_Trace;
}

KernelTraceSystem & Environment::getKernelTraceSystem() {
  return *m_KernelTraceSystem;
} 

AndroidTraceSystem & Environment::getAndroidTraceSystem() {
  return *m_AndroidTraceSystem;
}
 
TraceBuffer & Environment::getTraceBuffer() {
  return *m_TraceBuffer;
}

FileSystem & Environment::getFileSystem() {
  return *m_FileSystem;
}

ToolBox & Environment::getToolBox() {
  return *m_ToolBox;
}