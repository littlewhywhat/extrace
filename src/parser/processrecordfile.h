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

#ifndef LTTLWHWHT_PROCESSRECORDFILE_H
#define LTTLWHWHT_PROCESSRECORDFILE_H

#include "processchangefile.h"
#include "processrecord.h"

#include <string>
#include <vector>

using namespace std;

class ProcessRecordFile {
  public:
    virtual ~ProcessRecordFile() {}
    virtual void parseTo(vector<ProcessRecord*> & records) = 0;
};

#endif // LTTLWHWHT_PROCESSRECORDFILE_H

#ifndef LTTLWHWHT_SIMPLEPROCESSRECORDFILE_H
#define LTTLWHWHT_SIMPLEPROCESSRECORDFILE_H

class SimpleProcessRecordFile : public ProcessRecordFile {
  public:
    SimpleProcessRecordFile(const string & filename,
                            ProcessChangeFileCreator * processChangeFileCreator):
                            myFilename(filename),
                            myProcessChangeFileCreator(processChangeFileCreator) {}
    ~SimpleProcessRecordFile();
    void parseTo(vector<ProcessRecord*> & records) override;
  private:
    const string & myFilename;
    ProcessChangeFileCreator * myProcessChangeFileCreator;
};

#endif // LTTLWHWHT_SIMPLEPROCESSRECORDFILE_H
