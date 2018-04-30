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

#ifndef LTTLWHWHT_SYSTEMCORE_H
#define LTTLWHWHT_SYSTEMCORE_H

#include <cstdio>

#include "toolbox.h"
#include "filesystem.h"
#include "androidsystem.h"
#include "kernelsystem.h"
#include "trace.h"
#include "systemtime.h"
#include "signal.h"
#include "extracearguments.h"

class SystemCore
{
  public:
    ~SystemCore();
    FILE * getErrorStream() const;
    FILE * getOutputStream() const;
    ToolBox * getToolBox() const;
    FileSystem * getFileSystem() const;
    KernelSystem * getKernelSystem() const;
    AndroidSystem * getAndroidSystem() const;
    Trace * getTrace() const;
    SystemTime * getSystemTime() const;
    class Builder {
      public:
        SystemCore * build(const ExtraceArguments & arguments) const;
    };
  private:
    SystemCore();
    FILE          * m_ErrorStream   = NULL;
    FILE          * m_OutputStream  = NULL;
    ToolBox       * m_ToolBox       = NULL;
    FileSystem    * m_FileSystem    = NULL;
    AndroidSystem * m_AndroidSystem = NULL;
    KernelSystem  * m_KernelSystem  = NULL;
    Trace         * m_Trace         = NULL;
    SystemTime    * m_SystemTime    = NULL;

    void setErrorStream(FILE * errorStream);
    void setOutputStream(FILE * outputStream);
    void setToolBox(ToolBox * toolBox);
    void setFileSystem(FileSystem * fileSystem);
    void setKernelSystem(KernelSystem * kernelSystem);
    void setAndroidSystem(AndroidSystem * androidSystem);
    void setTrace(Trace * trace);
    void setSystemTime(SystemTime * systemTime);
};

#endif // LTTLWHWHT_SYSTEMCORE_H