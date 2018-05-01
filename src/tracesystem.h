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

#ifndef LTTLWHWHT_TRACESYSTEM_H
#define LTTLWHWHT_TRACESYSTEM_H

#include "filesystem.h"
#include "kernelsystem.h"
#include "androidsystem.h"
#include "trace.h"

class TraceSystem {
  public:
    TraceSystem(FileSystem * fileSystem,
                KernelSystem * kernelSystem,
                AndroidSystem * androidSystem,
                Trace * trace): m_FileSystem(fileSystem),
                                m_KernelSystem(kernelSystem),
                                m_AndroidSystem(androidSystem),
                                m_Trace(trace) {}
    ~TraceSystem();
    FileSystem & getFileSystem() const; 
    KernelSystem & getKernelSystem() const;
    AndroidSystem & getAndroidSystem() const;
    Trace & getTrace() const;
  private:
    FileSystem * m_FileSystem = NULL;
    KernelSystem * m_KernelSystem = NULL;
    AndroidSystem * m_AndroidSystem = NULL;
    Trace * m_Trace = NULL;
};

#endif // LTTLWHWHT_TRACESYSTEM_H