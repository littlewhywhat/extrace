/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef LTTLWHWHT_FILESYSTEM_H
#define LTTLWHWHT_FILESYSTEM_H

#include <stdio.h>

class FileSystem {
  public:
    virtual ~FileSystem() {}
    virtual bool fileExists(const char* filename) = 0;
    virtual bool fileIsWritable(const char* filename) = 0;
    virtual bool truncateFile(const char* path) = 0;
    virtual bool writeStr(const char* filename, const char* str) = 0;
    virtual bool appendStr(const char* filename, const char* str) = 0;
};

#endif // LTTLWHWHT_FILESYSTEM_H