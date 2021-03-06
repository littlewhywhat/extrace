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

#include "interpretdumpfileaction.h"

#include <vector>
#include <string>
#include <map>
#include <queue>
#include <inttypes.h>

#include "process.h"
#include "processrecordfile.h"
#include "simpleprocesschangefile.h"

using namespace std;

InterpretDumpFileAction::~InterpretDumpFileAction() {
  for (auto * record: myRecords) {
    delete record;
  }
  delete myUSSFilter;
}

InterpretDumpFileAction::InterpretDumpFileAction(const Wire & wire,
                                                 const shared_ptr<Environment> & environment,
                                                 const string & inputFile,
                                                 const vector<int> pids,
                                                 int cpuLimit,
                                                 USSFilter * ussFilter):
                                                 EnvironmentAction(wire, environment),
                                                 myInputFile(inputFile),
                                                 myCpuLimit(cpuLimit),
                                                 myUSSFilter(ussFilter) {
  for (int pid : pids) {
    myPIDs.insert(pid);
  }
}

bool InterpretDumpFileAction::tryRun() {
  SimpleProcessRecordFile file(myInputFile, new SimpleProcessChangeFileCreator());
  file.parseTo(myRecords);
  map<int, queue<ProcessRecord*>> sequences;

  for (auto * record : myRecords) {
    int pid = record->getPID();
    if (myPIDs.find(pid) != myPIDs.end()) { 
      if (sequences.find(pid) == sequences.end()) {
        sequences[pid] = queue<ProcessRecord*>();
      }
      sequences[pid].push(record);
    }
  }

  for (auto & pidAndQueue : sequences) {
    auto & records = pidAndQueue.second;
    size_t size;

    size = records.size();
    // apply user CPU filter
    for (size_t i = 0; i < size; i++) {
      auto * record = records.front();
      records.pop();
      if (!record->hasCpuUse()) {
        continue;      
      }  
      else {
        if (record->getCpuUse() >= myCpuLimit) {
          records.push(record);
        }
      }
    }

    // apply user Mem filter
    if (myUSSFilter) {
      myUSSFilter->filter(records);
    }
  }

  // print what left
  int lineLen;
  for (auto & pidAndQueue : sequences) {
    int pid = pidAndQueue.first;
    fprintf(m_Wire.getOutputStream(), "Sequence of events for PID = %d\n", pid);
    lineLen = fprintf(m_Wire.getOutputStream(), " * %5s | %3s  | %7s  | %7s  | %7s  | %7s  | %11s | %8s |\n",
                                      "PID", "CPU", "VSS", "RSS", "PSS", "USS", "TIMESTAMP", "STATE");
    fprintf(m_Wire.getOutputStream(), "%s\n", string(lineLen, '-').c_str());
    auto & queue = pidAndQueue.second;
    map<ProcessState, string> stateNames;
    stateNames = {
      { ProcessState::RUNNING, "RUNNING" },
      { ProcessState::SLEEPING, "SLEEPING"},
      { ProcessState::AWAKE, "AWAKE"},
      { ProcessState::UNKNOWN, "UNKNOWN"}
    };
    while (!queue.empty()) {
      auto * record = queue.front();


        fprintf(m_Wire.getOutputStream(), " | %5d | %3d %c |" " %7" PRIu64 "K | %7" PRIu64 "K | %7" PRIu64 "K | %7" PRIu64 "K | %11" PRIu64 " | %8s |\n",
                                        record->getPID(),
                                        record->getCpuUse(), '%',
                                        record->getVss() / 1024,
                                        record->getRss() / 1024,
                                        record->getPss() / 1024,
                                        record->getUss() / 1024,
                                        record->getTimeStamp(),
                                        stateNames[record->getState()].c_str());
        fprintf(m_Wire.getOutputStream(), "%s\n", string(lineLen, '-').c_str());
        fprintf(m_Wire.getOutputStream(), " | %s\n", record->getCause().c_str());
        fprintf(m_Wire.getOutputStream(), "%s\n", string(lineLen, '-').c_str());
      queue.pop();
    }
  }
  return true;
}