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

#ifndef LTTLWHWHT_INTERPRET_DUMPFILEACTION_H
#define LTTLWHWHT_INTERPRET_DUMPFILEACTION_H

#include "traceaction.h"

#include <string>
#include <memory>
#include <vector>

#include "processrecord.h"
#include "ussfilter.h"

using namespace std;

//! I am an EnvironmentAction that interpret file contents with uncompressed dump of trace buffer
class InterpretDumpFileAction : public EnvironmentAction {
  public:
    //! Constructs me
    InterpretDumpFileAction(const Wire & wire,
                            const shared_ptr<Environment> & environment,
                            const string & inputFile,
                            const vector<int> pids,
                            int cpuLimit,
                            USSFilter * ussFilter);
    //! Destroys my records and me
    ~InterpretDumpFileAction();
    bool tryRun() override;
  private:
    //! My input file to read dump from
    string myInputFile;
    //! My PIDs to focus on
    set<int> myPIDs;
    //! My Process records that I collect from input file
    vector<ProcessRecord*> myRecords;
    int myCpuLimit;
    USSFilter * myUSSFilter = NULL;
};

#endif // LTTLWHWHT_INTERPRET_DUMPFILEACTION_H