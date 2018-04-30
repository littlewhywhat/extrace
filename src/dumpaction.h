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

#ifndef LTTLWHWHT_DUMPACTION_H
#define LTTLWHWHT_DUMPACTION_H

#include "action.h"

#include <string>
#include <cstdio>

#include "kernelsystem.h"
#include "systemcore.h"
#include "extracearguments.h"

using namespace std;

class DumpAction : public Action {
  public:
    bool tryRun() override;
    void setKernelSystem(KernelSystem * kernelSystem);
    void setErrorStream(FILE * errorStream);
    void setOutputFile(const char * outputFile);
    void setOutputStream(FILE * outStream);
    void enableCompression();
    class Builder 
    {
      public:
        Action * buildFrom(const SystemCore & systemCore,
                           const ExtraceArguments & arguments) const;
    };
  private:
    KernelSystem * m_KernelSystem;
    FILE * m_ErrorStream;
    FILE * m_OutStream;
    string m_OutputFile;
    bool m_Compress = false;
};

#endif // LTTLWHWHT_DUMPACTION_H