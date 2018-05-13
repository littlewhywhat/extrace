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

#ifndef LTTLWHWHT_MEMORYSAMPLEACTION_H
#define LTTLWHWHT_MEMORYSAMPLEACTION_H

#include "interruptabletraceaction.h"

#include "kernel.h"
#include "process_mem.h"
#include "memoryusage.h"

#include <memory>
#include <stdint.h>

class MemorySampleAction : public InterruptableEnvironmentAction {
  public:
    MemorySampleAction(const Wire & wire, const shared_ptr<Environment> & environment,
                       const Signal & signal, uint32_t period, uint32_t times,
                       const vector<int> & pids):
                  InterruptableEnvironmentAction(wire, environment, signal),
                  m_Period(period),
                  m_Times(times),
                  myPIDs(pids) {}
    //! Removes my memory usages and processes during run if any
    ~MemorySampleAction();
    bool tryRun() override;
  private:
    uint32_t m_Period = 0;
    uint32_t m_Times = 0;
    Kernel * m_Kernel = NULL;
    vector<Process*> myProcesses;
    vector<MemoryUsage*> myMemoryUsages;
    vector<int> myPIDs;
};

#endif // LTTLWHWHT_MEMORYSAMPLEACTION_H