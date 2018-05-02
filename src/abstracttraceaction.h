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

#ifndef LTTLWHWHT_ABSTRACTTRACEACTION_H
#define LTTLWHWHT_ABSTRACTTRACEACTION_H

#include "traceaction.h"

#include "wire.h"

class AbstractTraceAction : public TraceAction {
  public:
    AbstractTraceAction(const Wire & wire): m_Wire(wire) {}
    virtual ~AbstractTraceAction() {}
  protected:
    const Wire & m_Wire;
};

#endif // LTTLWHWHT_ABSTRACTTRACEACTION_H
