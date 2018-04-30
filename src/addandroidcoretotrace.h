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

#ifndef LTTLWHWHT_ADDANDROIDCORETOTRACE_H
#define LTTLWHWHT_ADDANDROIDCORETOTRACE_H

#include "action.h"

#include <cstdio>

#include "trace.h"
#include "androidsystem.h"
#include "toolbox.h"
#include "systemcore.h"
#include "extracearguments.h"

using namespace std;

class AddAndroidCoreToTrace : public Action {
  public:
    bool tryRun() override;
    void setTrace(Trace * traceImpl);
    void setErrorStream(FILE * errorStream);
    void setToolBox(ToolBox * toolBox);
    void setAndroidSystem(AndroidSystem * androidSystem);
    class Builder {
      public:
        Action * buildFrom(const SystemCore & systemCore) const;
    };
  private:
    Trace * m_Trace;
    AndroidSystem * m_AndroidSystem;
    ToolBox * m_ToolBox;
    FILE * m_ErrorStream;
};

#endif // LTTLWHWHT_ADDANDROIDCORETOTRACE_H