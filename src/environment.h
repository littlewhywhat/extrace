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

#ifndef LTTLWHWHT_ENVIRONMENT_H
#define LTTLWHWHT_ENVIRONMENT_H

#include "trace.h"
#include "kerneltracesystem.h"
#include "androidtracesystem.h"
#include "tracebuffer.h"
#include "filesystem.h"
#include "toolbox.h"

#include <memory>

using namespace std;

//! I am an environment that provides all necessary components for EnvironmentAction to run.
class Environment {
  public:
    //! Creates me with new components
    Environment(const shared_ptr<Trace> & trace,
                const shared_ptr<KernelTraceSystem> & kernelTraceSystem,
                const shared_ptr<AndroidTraceSystem> & androidTraceSystem, 
                const shared_ptr<TraceBuffer> & traceBuffer,
                const shared_ptr<FileSystem> & fileSystem,
                const shared_ptr<ToolBox> & toolBox);
    //! Destroys me and my components
    ~Environment() {}
    //! Gets my Trace
    Trace & getTrace();
    //! Gets my KernelTraceSystem
    KernelTraceSystem & getKernelTraceSystem();
    //! Gets my AndroidTraceSystem
    AndroidTraceSystem & getAndroidTraceSystem();
    //! Gets my TraceBuffer
    TraceBuffer & getTraceBuffer();
    //! Gets my FileSystem
    FileSystem & getFileSystem();
    //! Gets my ToolBox
    ToolBox & getToolBox();
  private:
    //! My trace
    shared_ptr<Trace> m_Trace;
    //! My trace system of kernel
    shared_ptr<KernelTraceSystem> m_KernelTraceSystem;
    //! My trace system of android
    shared_ptr<AndroidTraceSystem> m_AndroidTraceSystem;
    //! My trace buffer
    shared_ptr<TraceBuffer> m_TraceBuffer;
    //! My file system
    shared_ptr<FileSystem> m_FileSystem;
    //! My toolbox
    shared_ptr<ToolBox> m_ToolBox;
};

#endif // LTTLWHWHT_ENVIRONMENT_H