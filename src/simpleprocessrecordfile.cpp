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

#include "processrecordfile.h"

#include "simpleprocess.h"

#include <map>

SimpleProcessRecordFile::~SimpleProcessRecordFile() {
  delete myProcessChangeFileCreator;
}

void SimpleProcessRecordFile::parseTo(vector<ProcessRecord*> & records) {
  ProcessChangeFile * file = myProcessChangeFileCreator->create(myFilename);
  vector<ProcessChange *> changes;
  file->parseTo(changes);
  
  if (changes.empty()) {
    return;
  }

  uint64_t startTime = changes.front()->getTimeStamp();
  uint64_t currentTime, duration;
  map<int, SimpleProcess> processesPerPid;
  int currentPID;
  for (auto * change : changes) {
    currentTime = change->getTimeStamp();
    duration = currentTime - startTime;

    currentPID = change->getPID();
    if (processesPerPid.find(currentPID) == processesPerPid.end()) {
      processesPerPid[currentPID] = SimpleProcess();
    }
    auto & process = processesPerPid[currentPID];
    change->applyTo(process);

    auto * record = new ProcessRecord();
    record->setPID(currentPID);
    record->setTimeStamp(currentTime);
    record->setVss(process.getVSS());
    record->setRss(process.getRSS());
    record->setPss(process.getPSS());
    record->setUss(process.getUSS());
    record->setState(process.getState());
    if (duration) {
      record->setCpuUsage((process.getTotalRun() * 100)/duration);
    }
    if (!change->getCause().empty()) {
      record->setCause(change->getCause());
    }
    records.push_back(record);
  }
}
