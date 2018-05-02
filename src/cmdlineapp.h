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

#ifndef LTTLWHWHT_CMDLINEAPP_H
#define LTTLWHWHT_CMDLINEAPP_H

#include "wire.h"
#include "cmdlineargs.h"
#include "actioncmdlinebuilder.h"
#include "signal.h"

#include <cstdio>
#include <string>

class CmdLineApp {
  public:
    CmdLineApp(): m_Wire(new Wire(stdout, stderr)) {}
    ~CmdLineApp();
    int run();
    void setArgs(const CmdLineArgs * cmdLineArgs);
    void setActionCmdLineBuilder(ActionCmdLineBuilder * actionCmdLineBuilder);
    void handleSignal();
  private:
    Wire * m_Wire = NULL;
    const CmdLineArgs * m_CmdLineArgs = NULL;
    const ActionCmdLineBuilder * m_ActionCmdLineBuilder = NULL;
    Signal m_Signal;
};

#endif // LTTLWHWHT_CMDLINEAPP_H