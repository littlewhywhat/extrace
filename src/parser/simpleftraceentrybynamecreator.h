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

#ifndef LTTLWHWHT_SIMPLE_FTRACE_ENTRY_BY_NAME_CREATOR_H
#define LTTLWHWHT_SIMPLE_FTRACE_ENTRY_BY_NAME_CREATOR_H

#include "ftraceentrybynamecreator.h"

#include "ftraceentrycreator.h"
#include "schedswitchentrycreator.h"
#include "schedwakeupentrycreator.h"
#include "tracingmarkentrycreator.h"

#include <map>
#include <string>

using namespace std;

class SimpleFTraceEntryByNameCreator : public FTraceEntryByNameCreator {
  public:
    SimpleFTraceEntryByNameCreator();
    ~SimpleFTraceEntryByNameCreator();
    FTraceEntry * create(int pid, long timeLow, long timeHigh,
                         const char * entryName,
                         const char * content) const override;
  private:
    map<string, FTraceEntryCreator *> myFTraceEntryCreators;
};

SimpleFTraceEntryByNameCreator::SimpleFTraceEntryByNameCreator() {
  myFTraceEntryCreators["sched_switch"] = new SchedSwitchEntryCreator();
  myFTraceEntryCreators["sched_wakeup"] = new SchedWakeUpEntryCreator();
  myFTraceEntryCreators["tracing_mark_write"] = new TracingMarkEntryCreator();
}

SimpleFTraceEntryByNameCreator::~SimpleFTraceEntryByNameCreator() {
  for (const auto & creator : myFTraceEntryCreators) {
    delete creator.second;
  }
}

FTraceEntry * SimpleFTraceEntryByNameCreator::create(int pid,
                                   long timeLow, long timeHigh,
                                   const char * entryName,
                                   const char * content) const {
  if (myFTraceEntryCreators.find(entryName) == myFTraceEntryCreators.end()) {
    return nullptr;
  }
  return myFTraceEntryCreators.at(entryName)->create(pid, timeLow, timeHigh, content);
}

#endif // LTTLWHWHT_SIMPLE_FTRACE_ENTRY_BY_NAME_CREATOR_H