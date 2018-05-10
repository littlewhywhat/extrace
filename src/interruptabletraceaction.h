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

#ifndef LTTLWHWHT_INTERRUPTABLEENVIRONMENTACTION_H
#define LTTLWHWHT_INTERRUPTABLEENVIRONMENTACTION_H

#include "traceaction.h"

#include "signal.h"

//! I am an EnvironmentAction that can be interrupted by Signal
class InterruptableEnvironmentAction : public EnvironmentAction {
  public:
    //! Constructs me with Signal and Environment
    InterruptableEnvironmentAction(const Wire & wire, shared_ptr<Environment> environment,
                                   const Signal & signal):
                             EnvironmentAction(wire, environment), m_Signal(signal) {}
    virtual ~InterruptableEnvironmentAction() {}
  protected:
    //! My signal
    const Signal & m_Signal;
};

#endif // LTTLWHWHT_INTERRUPTABLEENVIRONMENTACTION_H