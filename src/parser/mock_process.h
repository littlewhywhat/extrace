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

#ifndef LTTLWHWHT_MOCK_PROCESS_H
#define LTTLWHWHT_MOCK_PROCESS_H

#include <gmock/gmock.h>

#include "process.h"

class MockProcess : public Process {
  public:
    MOCK_CONST_METHOD0(getTotalRun, long(void));

    MOCK_CONST_METHOD0(getVSS, long(void));
    MOCK_CONST_METHOD0(getRSS, long(void));
    MOCK_CONST_METHOD0(getPSS, long(void));
    MOCK_CONST_METHOD0(getUSS, long(void));

    MOCK_METHOD1(setVSS, Process * (long vss));
    MOCK_METHOD1(setRSS, Process * (long rss));
    MOCK_METHOD1(setPSS, Process * (long pss));
    MOCK_METHOD1(setUSS, Process * (long uss));

    MOCK_METHOD1(setState, Process * (const ProcessState & state));
    MOCK_CONST_METHOD0(getState, const ProcessState & (void));
    MOCK_METHOD1(updateTo, void(long time));
};

#endif // LTTLWHWHT_MOCK_PROCESS_H