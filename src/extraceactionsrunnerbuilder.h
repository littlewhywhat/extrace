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

#ifndef LTTLWHWHT_EXTRACEACTIONSRUNNERBUILDER_H
#define LTTLWHWHT_EXTRACEACTIONSRUNNERBUILDER_H

#include "wire.h"
#include "signal.h"
#include "extracearguments.h"
#include "actionsrunner.h"
#include "environmentbuilder.h"

#include <memory>

class ExtraceActionsRunnerBuilder {
  public:
    ExtraceActionsRunnerBuilder(EnvironmentBuilder * environmentBuilder):
                                m_EnvironmentBuilder(environmentBuilder) {}
    ~ExtraceActionsRunnerBuilder();
    ActionsRunner * build(const Wire & wire, Signal & signal,
                          const ExtraceArguments & traceArguments) const;
  private:
    EnvironmentBuilder * m_EnvironmentBuilder = NULL;
    void addStopActions(ActionsRunner & actionsRunner, const Wire & wire,
                        Signal & signal, const ExtraceArguments & extraceArguments,
                                            shared_ptr<Environment> & environmen) const;
};

#endif // LTTLWHWHT_EXTRACEACTIONSRUNNERBUILDER_H