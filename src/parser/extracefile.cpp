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

#include "extracefile.h"

#include "entry.h"
#include "entryfile.h"
#include "processstat.h"

#include <map>

bool ExtraceFile::tryParseTo(vector<ProcessStat> & procStats) const {
  EntryFile * entriesFile = myEntryFileCreator->create(myFilename);

  vector<Entry*> entries;
  entriesFile->parseTo(entries);
  // NOTE some events - like switch could be relevant to two process - switched from and switched to
  // TODO Insert another layer to convert entries to memory and cpu usage ... somehow
  map<int, ProcessStat> procStatsPerPid;
  for (auto * entry : entries) {
    if (procStatsPerPid.find(entry->getPID()) == procStatsPerPid.end()) {
      ProcessStat procStat;
      procStat.setPID(entry->getPID());
      procStatsPerPid[entry->getPID()] = procStat;
    }
    auto & procStat = procStatsPerPid[entry->getPID()];
    // double dispatch - could be MemoryEntry, Switch Entry and so on
    // update is virtual for ExtraceFileEntry and they call procState.updateWith that is overloaded
    entry->update(procStat);
    // if all fields are filled
    if (procStat.isReady()) {
      procStats.push_back(procStat);
    }
  }
  return true;
}
