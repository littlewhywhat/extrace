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

#ifndef LTTLWHWHT_SIMPLE_FTRACE_ENTRY_FILE_H
#define LTTLWHWHT_SIMPLE_FTRACE_ENTRY_FILE_H

#include <errno.h>  // errno
#include <string.h> // strerror
#include <unistd.h> // access
#include <fcntl.h>  // creat, open
#include <cstdio>

#include <string>

using namespace std;

#include "ftraceentryfile.h"

//! idea is to update process stats with entries until stats are ready
//! NOTE some events - like switch could be relevant to two process - switched from and switched to
//! this should create two entries - SwitchFrom and SwitchTo
class SimpleFTraceEntryFile : public FTraceEntryFile {
  public:
    SimpleFTraceEntryFile(const string & filename,
                          const FTraceEntryByNameCreator * entryCreator):
                          myFilename(filename),
                          myEntryCreator(entryCreator) {}
    ~SimpleFTraceEntryFile();
    void parseTo(vector<FTraceEntry*> & entries) const override;
  private:
    const string & myFilename;
    const FTraceEntryByNameCreator * myEntryCreator = NULL;
};

#endif // LTTLWHWHT_SIMPLE_FTRACE_ENTRY_FILE_H