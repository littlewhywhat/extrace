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

#ifndef LTTLWHWHT_MOCK_FTRACEENTRY_FILE_H
#define LTTLWHWHT_MOCK_FTRACEENTRY_FILE_H

#include <gmock/gmock.h>

#include "ftraceentryfile.h"
#include "ftraceentryfilecreator.h"

class MockFTraceEntryFile : public FTraceEntryFile {
  public:
    MOCK_CONST_METHOD1(parseTo, void(vector<FTraceEntry*> & entries));
};

class MockFTraceEntryFileCreator : public FTraceEntryFileCreator {
  public:
    MOCK_CONST_METHOD1(create, FTraceEntryFile*(const string & filename));
};

#endif // LTTLWHWHT_MOCK_FTRACEENTRY_FILE_H