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

#ifndef LTTLWHWHT_EXTRACE_FILE_H
#define LTTLWHWHT_EXTRACE_FILE_H

#include "entryfilecreator.h"

#include <string>

//! I am an extrace trace file. I can be parsed to vector of ProcessStat.
class ExtraceFile {
  public:
    ExtraceFile(const string & filename,
                EntryFileCreator * fileCreator):
                myFilename(filename),
                myEntryFileCreator(fileCreator) {}
    // TODO destructor
    bool tryParseTo(vector<ProcessStat> & procStats) const;
  private:
    const string & myFilename;
    EntryFileCreator * myEntryFileCreator = NULL;
};

#endif // PARSER_H