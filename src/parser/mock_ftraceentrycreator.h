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

#ifndef LTTLWHWHT_MOCK_FTRACE_ENTRY_CREATOR_H
#define LTTLWHWHT_MOCK_FTRACE_ENTRY_CREATOR_H

#include <gmock/gmock.h>

#include "ftraceentrybynamecreator.h"

//! I am a fake Entry creator. I don't really create Entry but just pass mine  
class MockFTraceEntryCreator : public FTraceEntryByNameCreator {
  public:
    MOCK_CONST_METHOD5(create, FTraceEntry*(int pid, long timeLow, long timeHigh,
                                            const char * entryName,
                                            const char * content));
};

#endif // LTTLWHWHT_MOCK_FTRACE_ENTRY_CREATOR_H