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

#ifndef LTTLWHWHT_CLEANUPACTION_H
#define LTTLWHWHT_CLEANUPACTION_H

#include "abstracttraceaction.h"

#include "wire.h"
#include "environment.h"
#include "tracesystem.h"

class CleanUpAction : public AbstractTraceAction {
  public:
    CleanUpAction(const Wire & wire): AbstractTraceAction(wire) {}
    bool tryRunIn(Environment & environment, TraceSystem & traceSystem) override;
};

#endif // LTTLWHWHT_CLEANUPACTION_H