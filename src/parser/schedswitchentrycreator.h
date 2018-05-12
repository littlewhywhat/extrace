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

#ifndef LTTLWHWHT_SCHED_SWITCH_ENTRY_CREATOR_H
#define LTTLWHWHT_SCHED_SWITCH_ENTRY_CREATOR_H

#include "entrycreator.h"

//! I am an EntryCreator from sched_switch content
class SchedSwitchEntryCreator {
  public:
    ~SchedSwitchEntryCreator() {}
    Entry * create(int pid, long timeLow, long timeHigh, const char * content) const;
};

Entry * SchedSwitchEntryCreator::create(int pid, long timeLow, long timeHigh,
                                        const char * content) const {
  return nullptr;
}

#endif // LTTLWHWHT_SCHED_SWITCH_ENTRY_CREATOR_H
