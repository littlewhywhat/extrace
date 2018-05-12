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

#include "ftraceentry.h"

uint64_t FTraceEntry::myTime() const {
  return (uint64_t)myTimeLow + (1000000l * (uint64_t)myTimeHigh);
}

void SchedWakeUpEntry::parseTo(vector<ProcessChange*> & procChanges) const {
  procChanges.push_back((new StateChange(myWakedUpPID, myTime()))
                            ->setState(ProcessState::AWAKE));
}

void SchedSwitchEntry::parseTo(vector<ProcessChange*> & procChanges) const {
  procChanges.push_back((new StateChange(myPrevPID, myTime()))
                            ->setState(ProcessState::SLEEPING));
  procChanges.push_back((new StateChange(myNextPID, myTime()))
                            ->setState(ProcessState::RUNNING));
}

void TracingMarkEntry::parseTo(vector<ProcessChange*> & procChanges) const {
  procChanges.push_back((new MemoryChange(myTracedPID, myTime()))
                            ->setVSS(myVSS)
                            ->setUSS(myUSS)
                            ->setRSS(myRSS)
                            ->setPSS(myPSS));
}
