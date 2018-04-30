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

#ifndef LTTLWHWHT_STARTACTION_H
#define LTTLWHWHT_STARTACTION_H

#include "action.h"

#include "trace.h"
#include "kernelsystem.h"
#include "systemcore.h"

using namespace std;

class StartAction : public Action {
  public:
    bool tryRun() override;
    void setTrace(Trace * trace);
    void setErrorStream(FILE * errorStream);
    void setOutputStream(FILE * outStream);
    void setKernelSystem(KernelSystem * kernelSystem);
    class Builder {
      public:
        Action * buildFrom(const SystemCore & systemCore) const;
    };
  private:
    Trace * m_Trace;
    KernelSystem * m_KernelSystem;
    FILE * m_ErrorStream;
    FILE * m_OutStream;
};

#endif // LTTLWHWHT_STARTACTION_H