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

/* This is a template for MockFileSystem
   Things to substitute:
    - FileSystem, filesystem, FILESYSTEM
    - WRITESTR, writeStr
 */

#ifndef LTTLWHWHT_MOCK_FILESYSTEM_H
#define LTTLWHWHT_MOCK_FILESYSTEM_H

#include <gmock/gmock.h>

#include "filesystem.h"

class MockFileSystem : public FileSystem {
  public:
    MOCK_METHOD1(tryOpenFileToWriteOrCreate, int(const char* filename));
    MOCK_METHOD1(fileExists, bool(const char* filename));
    MOCK_METHOD1(fileIsWritable, bool(const char* filename));
    MOCK_METHOD1(truncateFile, bool(const char* path));
    MOCK_METHOD2(writeStr, bool(const char* filename, const char* str));
    MOCK_METHOD2(appendStr, bool(const char* filename, const char* str));
    MOCK_METHOD3(readStr, bool(const char* filename, char* str, int max_size));
};

#endif // LTTLWHWHT_MOCK_FILESYSTEM_H