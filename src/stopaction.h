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

#ifndef LTTLWHWHT_STOPACTION_H
#define LTTLWHWHT_STOPACTION_H

#include "action.h"

#include <cstdio>

#include "trace.h"
#include "systemcore.h"

using namespace std;

class StopAction : public Action {
  public:
    bool tryRun() override;
    void setTrace(Trace * trace);
    void setErrorStream(FILE * errorStream);
    class Builder {
      public:
        Action * buildFrom(const SystemCore & systemCore) const;
    };
  private:
    Trace * m_Trace;
    FILE * m_ErrorStream;
};

#endif // LTTLWHWHT_STOPACTION_H