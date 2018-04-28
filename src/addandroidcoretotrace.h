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

#include <memory>
#include <cstdio>

#include "trace_impl.h"
#include "androidsystem.h"
#include "toolbox.h"

using namespace std;

class AddAndroidCoreToTrace : public Action {
  public:
    bool tryRun() override;
    void setTraceImpl(shared_ptr<TraceImpl> & traceImpl);
    void setErrorStream(FILE * errorStream);
    void setToolbox(shared_ptr<Toolbox> & toolBox);
    void setAndroidSystem(shared_ptr<AndroidSystem> & androidSystem);
    class Builder {
      public:
        Builder(FILE * errorStream,
                shared_ptr<TraceImpl> & traceImpl, 
                shared_ptr<AndroidSystem> & androidSystem,
                shared_ptr<Toolbox> & toolBox); 
        AddAndroidCoreToTrace * build() const;
      private:
        AddAndroidCoreToTrace * m_AddAndroidCoreToTrace;
    };
  private:
    shared_ptr<TraceImpl> m_TraceImpl;
    shared_ptr<AndroidSystem> m_AndroidSystem;
    shared_ptr<Toolbox> m_ToolBox;
    FILE * m_ErrorStream;
};

#endif // LTTLWHWHT_ADDANDROIDCORETOTRACE_H