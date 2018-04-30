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

#include "actionrunner_impl.h"

ActionRunnerImpl::~ActionRunnerImpl() 
{
  delete m_SystemCore;
  for (auto * action : m_Actions)
  {
    delete action;
  }
}

void ActionRunnerImpl::setSystemCore(SystemCore * systemCore)
{
  m_SystemCore = systemCore;
}

bool ActionRunnerImpl::tryRunActions() {
  for (Action * action: m_Actions) {
    if (!action->tryRun()) {
      return false;
    }
  }
  return true;
}

void ActionRunnerImpl::addAction(Action * action) {
  m_Actions.push_back(action);
}

void ActionRunnerImpl::addInterruptableAction(InterruptableAction * action) {
  m_InterruptableActions.push_back(action);
  m_Actions.push_back(action);
}

void ActionRunnerImpl::handleSignal() {
  if (m_InterruptsEnabled) {
    for (auto * action : m_InterruptableActions) {
      action->handleSignal();
    }
  }
}

void ActionRunnerImpl::enableInterrupts() {
  m_InterruptsEnabled = true;  
}
