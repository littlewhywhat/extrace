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

#ifndef LTTLWHWHT_MOCK_KERNELSYSTEM_H
#define LTTLWHWHT_MOCK_KERNELSYSTEM_H

#include <gmock/gmock.h>

#include "kernelsystem.h"

class MockKernelSystem : public KernelSystem {
  public:
    MOCK_METHOD1(tryOpenToWriteOrCreate, int(const char* filename));
    MOCK_METHOD2(try_sendfile, bool(int fd_from, int fd_to));
    MOCK_METHOD0(writeClockSyncMarker, bool(void));
    MOCK_METHOD1(setTraceOverwriteEnable, bool(bool enable));
    MOCK_METHOD1(setTracingEnabled, bool(bool enable));
    MOCK_METHOD0(clearTrace, bool(void));
    MOCK_METHOD0(getTracePipeFd, int(void));
    MOCK_METHOD0(getTraceFd, int(void));
    MOCK_METHOD2(try_send, bool(int fd_from, int fd_to));
    MOCK_METHOD1(setTraceBufferSizeKB, bool(int size));
    MOCK_METHOD1(setGlobalClockEnable, bool(bool enable));
    MOCK_METHOD1(setPrintTgidEnableIfPresent, bool(bool enable));
    MOCK_METHOD1(setKernelTraceFuncs, bool(const vector<string> & funcs));
    MOCK_METHOD1(enableKernelTraceEvents, bool(const vector<string> & ids));
    MOCK_CONST_METHOD0(getCategories, const vector<TracingCategory>&(void));
    MOCK_METHOD0(disableKernelTraceEvents, bool());
    MOCK_CONST_METHOD1(isCategorySupported, bool(const TracingCategory& category));
};

#endif // LTTLWHWHT_MOCK_KERNELSYSTEM_H