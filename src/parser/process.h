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

enum ProcessState {
  RUNNING,
  SLEEPING,
  AWAKE,
  UNKNOWN
};

//! I am a process. I can run, sleep and wakeup waiting until running. I can record my statistics in ProcessRecord.

//! I know my current memory usage and how much I ran in total from creation
//! I update my total by provided time
class Process {
  public:
    long getTotalRun() { return myTotalRun; }

    long getVSS() const { return myVSS; }
    long getRSS() const { return myRSS; }
    long getPSS() const { return myPSS; }
    long getUSS() const { return myUSS; }

    // TODO this should be accessed only via ProcessChange
    Process * setVSS(long vss) { myVSS = vss; return this; }
    Process * setRSS(long rss) { myRSS = rss; return this; }
    Process * setPSS(long pss) { myPSS = pss; return this; }
    Process * setUSS(long uss) { myUSS = uss; return this; }

    Process * setState(const ProcessState & state) { myState = state; return this; }
    const ProcessState & getState() const { return myState; }
    //! Updates my time statistics

    //! Updates my total run if myLastUpdateTime
    //! is defined and I am ProcessState::RUNNING.
    //! Sets myLastUpdateTime to specified time.
    void updateTo(long time);
  private:
    ProcessState myState = ProcessState::UNKNOWN;
    long myTotalRun = 0;
    long myLastUpdateTime = -1;
    long myVSS = -1;
    long myRSS = -1;
    long myPSS = -1;
    long myUSS = -1;
};

#endif // LTTLWHWHT_PROCESS_H