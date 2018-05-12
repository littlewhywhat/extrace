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

#ifndef LTTLWHWHT_MOCK_PROCESSCHANGE_FILE_H
#define LTTLWHWHT_MOCK_PROCESSCHANGE_FILE_H

#include <gmock/gmock.h>

#include "processchangefile.h"
#include "processchangefilecreator.h"

class MockProcessChangeFile : public ProcessChangeFile {
  public:
    MOCK_CONST_METHOD1(parseTo, void(vector<ProcessChange*> & entries));
};

class MockProcessChangeFileCreator : public ProcessChangeFileCreator {
  public:
    MOCK_CONST_METHOD1(create, ProcessChangeFile*(const string & filename));
};

#endif // LTTLWHWHT_MOCK_PROCESSCHANGE_FILE_H