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

#ifndef LTTLWHWHT_EMPTYENTRY_CREATOR_H
#define LTTLWHWHT_EMPTYENTRY_CREATOR_H

#include "ftraceentrycreator.h"

//! I am an EntryCreator from tracing_mark content starting with letter E
class EmptyEntryCreator : public FTraceEntryCreator {
  public:
    ~EmptyEntryCreator() {}
    FTraceEntry * create(int pid, long timeLow, long timeHigh, const char * content) const override;
};

#endif // LTTLWHWHT_EMPTYENTRY_CREATOR_H