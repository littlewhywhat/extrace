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

#include "entry.h"

void Entry::update(ProcessStat & procStat) const {
  procStat.setPID(myPID);
  procStat.setTimeHigh(myTimeHigh);
  procStat.setTimeLow(myTimeLow);
  updateFurther(procStat);
}

int Entry::getPID() const {
  return myPID;
}

Entry* Entry::setPID(int pid) {
  myPID = pid;
  return this;
}

int     Entry::getTimeHigh() const {
  return myTimeHigh;
}

Entry * Entry::setTimeHigh(int time) {
  myTimeHigh = time;
  return this;
}

int     Entry::getTimeLow() const {
  return myTimeLow;
}

Entry * Entry::setTimeLow(int time) {
  myTimeLow = time;
  return this;
}

void MemoryEntry::updateFurther(ProcessStat & procStat) const {
  procStat.updateWith(*this);
}

void CpuUsageEntry::updateFurther(ProcessStat & procStat) const {
  procStat.updateWith(*this);
}
