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

#ifndef LTTLWHWHT_TRACEACTIONSRUNNERCMDLINEBUILDER_H
#define LTTLWHWHT_TRACEACTIONSRUNNERCMDLINEBUILDER_H

#include "actioncmdlinebuilder.h"

#include "action.h"
#include "wire.h"
#include "cmdlineargs.h"
#include "traceargumentsbuilder.h"
#include "tracesystembuilder.h"
#include "traceactionsrunnerbuilder.h"

class TraceActionsRunnerCmdLineBuilder : public ActionCmdLineBuilder {
  public:
    TraceActionsRunnerCmdLineBuilder(TraceArgumentsBuilder     * traceArgumentsBuilder,
                                     TraceSystemBuilder        * traceSystemBuilder,
                                     TraceActionsRunnerBuilder * traceActionsRunner):
                                     m_TraceArgumentsBuilder(traceArgumentsBuilder),
                                     m_TraceSystemBuilder(traceSystemBuilder),
                                     m_TraceActionsRunnerBuilder(traceActionsRunner) {}

    Action * build(const Wire & wire, const Signal & signal,
                   const CmdLineArgs & cmdLineArgs) const override;
  private:
    TraceArgumentsBuilder * m_TraceArgumentsBuilder = NULL;
    TraceSystemBuilder    * m_TraceSystemBuilder = NULL;
    TraceActionsRunnerBuilder * m_TraceActionsRunnerBuilder = NULL;
};

#endif // LTTLWHWHT_TRACEACTIONSRUNNERCMDLINEBUILDER_H