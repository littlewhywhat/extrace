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

#ifndef LTTLWHWHT_PROCESS_H
#define LTTLWHWHT_PROCESS_H

#include "processstate.h"

//! I am a process. I can run, sleep and wakeup waiting until running. I can record my statistics in ProcessRecord.

//! I know my current memory usage and how much I ran in total from creation
//! I update my total by provided time
class Process {
  public:
    virtual ~Process() {}
    virtual long getTotalRun() const = 0;

    virtual long getVSS() const = 0;
    virtual long getRSS() const = 0;
    virtual long getPSS() const = 0;
    virtual long getUSS() const = 0;

    // TODO this should be accessed only via ProcessChange
    virtual Process * setVSS(long vss) = 0;
    virtual Process * setRSS(long rss) = 0;
    virtual Process * setPSS(long pss) = 0;
    virtual Process * setUSS(long uss) = 0;

    virtual Process * setState(const ProcessState & state) = 0;
    virtual const ProcessState & getState() const = 0;
    //! Updates my time statistics

    //! Updates my total run if myLastUpdateTime
    //! is defined and I am ProcessState::RUNNING.
    //! Sets myLastUpdateTime to specified time.
    virtual void updateTo(long time) = 0;
};

#endif // LTTLWHWHT_PROCESS_H