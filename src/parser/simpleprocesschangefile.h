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

#ifndef LTTLWHWHT_SIMPLEPROCESSCHANGEFILE_H
#define LTTLWHWHT_SIMPLEPROCESSCHANGEFILE_H

#include "processchangefile.h"

#include "ftraceentryfilecreator.h"

#include <string>

class SimpleProcessChangeFile : public ProcessChangeFile {
  public:
    SimpleProcessChangeFile(const string & filename,
                            FTraceEntryFileCreator * ftraceEntryFileCreator):
                            myFilename(filename),
                            myFTraceEntryFileCreator(ftraceEntryFileCreator) {}
    ~SimpleProcessChangeFile() {}
    void parseTo(vector<ProcessChange*> & procChanges) const override;
  private:
    string myFilename;
    FTraceEntryFileCreator * myFTraceEntryFileCreator;
};

#endif // LTTLWHWHT_SIMPLEPROCESSCHANGEFILE_H