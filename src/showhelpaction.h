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

#ifndef LTTLWHWHT_SHOWHELPACTION_H
#define LTTLWHWHT_SHOWHELPACTION_H

#include "action.h"

#include "environment.h"

class ShowHelpAction : public Action {
  public:
    ShowHelpAction(const Wire & wire, const shared_ptr<Environment> & environment,
                   const string & helpMessage): Action(wire), m_Environment(environment),
                   m_HelpMessage(helpMessage) {}
    bool tryRun() override;
  private:
    shared_ptr<Environment> m_Environment; 
    string m_HelpMessage;
};

#endif // LTTLWHWHT_SHOWHELPACTION_H