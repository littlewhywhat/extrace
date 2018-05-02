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

#ifndef LTTLWHWHT_ACTIONSRUNNER_H
#define LTTLWHWHT_ACTIONSRUNNER_H

#include "action.h"

#include <vector>

using namespace std;

class ActionsRunner : public Action {
  public:
    ActionsRunner(const Wire & wire): Action(wire) {}
    ~ActionsRunner();
    void addAction(Action * action);
    bool tryRun() override;
  private:
    vector<Action*> m_Actions;
};

#endif // LTTLWHWHT_ACTIONSRUNNER_H