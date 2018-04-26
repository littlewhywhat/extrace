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

#ifndef LTTLWHWHT_MOCK_FILESYSTEM_IMPL_H
#define LTTLWHWHT_MOCK_FILESYSTEM_IMPL_H

#include "gmock/gmock.h"

#include "filesystem_impl.h"

class MockFileSystemImpl : public FileSystemImpl {
  public:
    MOCK_METHOD1(set_errstream, void(FILE*));
    MOCK_METHOD1(fileExists, bool(const char*));
    MOCK_METHOD1(fileIsWritable, bool(const char*));
    MOCK_METHOD1(truncateFile, bool(const char*));
    MOCK_METHOD2(writeStr, bool(const char *, const char*));
    MOCK_METHOD2(appendStr, bool(const char *, const char*));
    MOCK_METHOD3(readStr, bool(const char *, char*, int));
};

#endif // LTTLWHWHT_MOCK_FILESYSTEM_IMPL_H