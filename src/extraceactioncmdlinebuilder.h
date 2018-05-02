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

#ifndef LTTLWHWHT_EXTRACEACTIONSRUNNERCMDLINEBUILDER_H
#define LTTLWHWHT_EXTRACEACTIONSRUNNERCMDLINEBUILDER_H

#include "actioncmdlinebuilder.h"

#include "extraceargumentsbuilder.h"
#include "extraceactionsrunnerbuilder.h"

class ExtraceActionCmdLineBuilder : public ActionCmdLineBuilder {
  public:
    ExtraceActionCmdLineBuilder(ExtraceArgumentsBuilder     * traceArgumentsBuilder,
                                ExtraceActionsRunnerBuilder * traceActionsRunner):
                                m_ExtraceArgumentsBuilder(traceArgumentsBuilder),
                                m_ExtraceActionsRunnerBuilder(traceActionsRunner) {}
    ~ExtraceActionCmdLineBuilder();
    Action * build(const Wire & wire, Signal & signal,
                   const CmdLineArgs & cmdLineArgs) const override;
  private:
    ExtraceArgumentsBuilder * m_ExtraceArgumentsBuilder = NULL;
    ExtraceActionsRunnerBuilder * m_ExtraceActionsRunnerBuilder = NULL;
};

#endif // LTTLWHWHT_EXTRACEACTIONSRUNNERCMDLINEBUILDER_H