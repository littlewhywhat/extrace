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

#ifndef LTTLWHWHT_PROCESS_CHANGE_FILE_H
#define LTTLWHWHT_PROCESS_CHANGE_FILE_H

#include "processchange.h"

#include <vector>
#include <string>

using namespace std;

class ProcessChangeFile {
  public:
    virtual ~ProcessChangeFile() {}
    virtual void parseTo(vector<ProcessChange*> & procChanges) const = 0;
};

class ProcessChangeFileCreator {
  public:
    virtual ~ProcessChangeFileCreator() {}
    virtual ProcessChangeFile * create(const string & filename) const = 0;    
};

#endif // LTTLWHWHT_PROCESS_CHANGE_FILE_H