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

#include "traceactionsrunnercmdlinebuilder.h"
#include "environment.h"

Action * TraceActionsRunnerCmdLineBuilder::build(const Wire & wire,
                                                 const string & appName,
                                                 const Signal & signal,
                                                 const CmdLineArgs & cmdLineArgs) const {
  auto   sp_traceArguments  = unique_ptr<const TraceArguments>(m_TraceArgumentsBuilder->build(wire, cmdLineArgs));
  auto * environment        = new Environment(signal, appName, (*sp_traceArguments).ignoreSignalsEnabled());
  auto * traceSystem        = m_TraceSystemBuilder->build(wire, *sp_traceArguments);
  auto * traceActionsRunner = m_TraceActionsRunnerBuilder->build(wire, environment, traceSystem, *sp_traceArguments);
  return traceActionsRunner;
}