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

#include "cmdlineapp.h"

#include <cstdlib>
#include <memory>

void CmdLineApp::handleSignal() {
  m_Signal.fire();
}

void CmdLineApp::setArgs(const CmdLineArgs * cmdLineArgs) {
  m_CmdLineArgs = cmdLineArgs;
}

void CmdLineApp::setActionCmdLineBuilder(ActionCmdLineBuilder * actionCmdLineBuilder) {
  m_ActionCmdLineBuilder = actionCmdLineBuilder;
}

CmdLineApp::~CmdLineApp() {
  delete m_CmdLineArgs;
  delete m_Wire;
}

int CmdLineApp::run() {
  if (!m_CmdLineArgs || !m_ActionCmdLineBuilder) {
    return EXIT_FAILURE;
  }
  auto action = unique_ptr<Action>(m_ActionCmdLineBuilder->build(*m_Wire, m_Signal, *m_CmdLineArgs));
  if (action->tryRun()) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
