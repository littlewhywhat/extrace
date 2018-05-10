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

#ifndef LTTLWHWHT_ENVIRONMENTACTION_H
#define LTTLWHWHT_ENVIRONMENTACTION_H

#include "action.h"

#include <memory>

#include "environment.h"

//! I am an Action that has access to Environment
class EnvironmentAction : public Action {
  public:
    //! Constructs me
    EnvironmentAction(const Wire & wire, shared_ptr<Environment> environment):
                Action(wire), m_Environment(environment) {}
    //! Destroys only me (not environment)
    virtual ~EnvironmentAction() {}
  protected:
    //! My environment
    shared_ptr<Environment> m_Environment;
};

#endif // LTTLWHWHT_ENVIRONMENTACTION_H