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

#include "traceactionsrunner.h"

TraceActionsRunner::~TraceActionsRunner() {
  delete m_TraceSystem;
  delete m_Environment;
  for (auto * traceAction : m_TraceActions) {
    delete traceAction;
  }
}

void TraceActionsRunner::addTraceAction(TraceAction * traceAction) {
  m_TraceActions.push_back(traceAction);
}

bool TraceActionsRunner::tryRun() {
  for (auto * traceAction : m_TraceActions) {
    if (!traceAction->tryRunIn(*m_Environment, *m_TraceSystem)) {
      return false;
    }
  }
  return true;
}