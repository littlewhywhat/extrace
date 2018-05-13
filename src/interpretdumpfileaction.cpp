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
#include <iostream>

#include "processrecord.h"
#include "processrecordfile.h"
#include "simpleprocesschangefile.h"

bool InterpretDumpFileAction::tryRun() {
  SimpleProcessRecordFile file(myInputFile, new SimpleProcessChangeFileCreator());
  vector<ProcessRecord*> records;
  file.parseTo(records);
  for (auto * record : records) {
    cout << *record << endl;
  }
  return true;
}