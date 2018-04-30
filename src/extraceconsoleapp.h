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
#ifndef LTTLWHWHT_EXTRACECONSAPP_H
#define LTTLWHWHT_EXTRACECONSAPP_H

#include "abstractconsoleapp.h"
#include "argsparser.h"
#include "arguments.h"
#include "extracearguments.h"
#include "actionrunner.h"
#include "actionrunnerbuilder.h"

#include <cstdio>
#include <string>

using namespace std;

//! I am a wrapper for extrace app to run from console
class ExtraceConsoleApp : public AbstractConsoleApp
{
  public:
    ExtraceConsoleApp(): AbstractConsoleApp() {}
    ~ExtraceConsoleApp();
    void handleSignal() override;
    void setActionRunnerBuilder(ActionRunnerBuilder * actionRunnerBuilder);
    void setErrorStream(FILE * errorStream);
   private:
    ExtraceArguments m_ExtraceArguments;
    ActionRunner * m_ActionRunner = NULL;
    ActionRunnerBuilder * m_ActionRunnerBuilder = NULL;
    bool m_Success = false;
    FILE * m_ErrorStream = NULL;
    void printHelp() const override;
    void do_run(void) override;
    bool do_succeeded(void) const override;
    void register_args_to(ArgsParser & argsParser) const override;
    bool trySetup(const Arguments & arguments) override;
};

#endif // LTTLWHWHT_EXTRACECONSAPP_H