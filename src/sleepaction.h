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

#ifndef LTTLWHWHT_SLEEPACTION_H
#define LTTLWHWHT_SLEEPACTION_H

#include "action.h"

#include <inttypes.h> // uint32_t
#include <stdio.h>    // FILE
#include <memory>

#include "signal.h"

using namespace std;

class SleepAction : public Action {
  public:
    bool tryRun() override;
    void setDurationSeconds(uint32_t durationSeconds);
    void setSignal(shared_ptr<Signal> & signal);
    void setErrorStream(FILE * errorStream);
  private:
    uint32_t m_DurationSeconds = 5;
    shared_ptr<Signal> m_Signal;
    FILE * m_ErrorStream;
};

#endif // LTTLWHWHT_SLEEPACTION_H