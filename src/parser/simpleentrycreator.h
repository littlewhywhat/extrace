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

#ifndef LTTLWHWHT_SIMPLE_ENTRY_CREATOR_H
#define LTTLWHWHT_SIMPLE_ENTRY_CREATOR_H

#include "simpleentryfile.h"
#include "entrycreator.h"
#include "schedswitchentrycreator.h"
#include "schedwakeupentrycreator.h"
#include "tracingmarkentrycreator.h"

class SimpleEntryByNameCreator : public EntryByNameCreator {
  public:
    SimpleEntryByNameCreator();
    Entry * create(int pid, long timeLow, long timeHigh,
                   const char * entryName,
                   const char * content) const override;
  private:
    map<string, EntryCreator *> myEntryCreators;
};

SimpleEntryByNameCreator::SimpleEntryByNameCreator() {
  myEntryCreators["sched_switch"] = new SchedSwitchEntryCreator();
  myEntryCreators["sched_wakeup"] = new SchedWakeUpEntryCreator();
  myEntryCreators["tracing_mark_write"] = new TracingMarkEntryCreator();
}

SimpleEntryByNameCreator::~SimpleEntryByNameCreator() {
  for (auto * creator : myEntryCreators) {
    delete creator;
  }
}

Entry * SimpleEntryCreator::create(int pid, long timeLow, long timeHigh,
                                   const char * entryName,
                                   const char * content) const {
  if (myEntryCreators.find(entryName) == myEntryCreators.end()) {
    return NULL;
  }
  return myEntryCreators[entryName].create(pid, timeLow, timeHigh, content);
}

#endif // LTTLWHWHT_SIMPLE_ENTRY_CREATOR_H

