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

#ifndef LTTLWHWHT_TRACINGMARKENTRY_CREATOR_H
#define LTTLWHWHT_TRACINGMARKENTRY_CREATOR_H

#include "ftraceentrycreator.h"

#include <map>

//! I am an EntryCreator from tracing_mark content
class TracingMarkEntryCreator : public FTraceEntryCreator {
  public:
    TracingMarkEntryCreator();
    ~TracingMarkEntryCreator();
    FTraceEntry * create(int pid, long timeLow, long timeHigh, const char * content) const;
  private:
    map<char, FTraceEntryCreator *> myFTraceEntryCreators;
};

#endif // LTTLWHWHT_TRACINGMARKENTRY_CREATOR_H
