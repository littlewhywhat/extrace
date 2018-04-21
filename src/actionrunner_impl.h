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

#ifndef LTTLWHWHT_ACTION_RUNNER_IMPL_H
#define LTTLWHWHT_ACTION_RUNNER_IMPL_H

#include "actionrunner.h"

#include <vector>

#include "action.h"

using namespace std;

class ActionRunnerImpl : ActionRunner {
  public:
    ~ActionRunnerImpl();
    bool tryRunActions() override;
    void addAction(Action * action);
  private:
    vector<Action*> m_Actions;
};

#endif // LTTLWHWHT_ACTION_RUNNER_IMPL_H