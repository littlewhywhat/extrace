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

#ifndef LTTLWHWHT_TRACEACTIONSRUNNERBUILDER_H
#define LTTLWHWHT_TRACEACTIONSRUNNERBUILDER_H

#include "wire.h"
#include "tracesystem.h"
#include "tracearguments.h"
#include "traceactionsrunner.h"
#include "environment.h"

class TraceActionsRunnerBuilder {
  public:
    TraceActionsRunner * build(const Wire & wire, Environment * environment,
                               TraceSystem * traceSystem, const TraceArguments & traceArguments) const;
};

#endif // LTTLWHWHT_TRACEACTIONSRUNNERBUILDER_H