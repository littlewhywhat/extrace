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
#include <sstream>

#include "processrecord.h"
#include "processrecordfile.h"
#include "simpleprocesschangefile.h"

using namespace std;

InterpretDumpFileAction::InterpretDumpFileAction(const Wire & wire,
                                                 const shared_ptr<Environment> & environment,
                                                 const string & inputFile, const vector<int> pids):
                                                 EnvironmentAction(wire, environment),
                                                 myInputFile(inputFile) {
  for (int pid : pids) {
    myPIDs.insert(pid);
  }
}

bool InterpretDumpFileAction::tryRun() {
  SimpleProcessRecordFile file(myInputFile, new SimpleProcessChangeFileCreator());
  vector<ProcessRecord*> records;
  file.parseTo(records);
  fprintf(m_Wire.getOutputStream(), "%5s | %3s | %10s | %10s | %10s | %10s | %11s | %8s | Cause\n",
                                  "PID", "CPU", "VSS", "RSS", "PSS", "USS", "TIMESTAMP", "STATE"
         );
  stringstream ss;
  for (auto * record : records) {
    if (myPIDs.find(record->getPID()) != myPIDs.cend()) {
      ss << *record;
      fprintf(m_Wire.getOutputStream(), "%s\n", ss.str().c_str());
      ss.str(string());
      ss.clear();
    }
  }
  return true;
}