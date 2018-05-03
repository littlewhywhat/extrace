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

#ifndef LTTLWHWHT_FILESYSTEM_IMPL_H
#define LTTLWHWHT_FILESYSTEM_IMPL_H

#include "filesystem.h"

#include "wire.h"

class FileSystemImpl : public FileSystem {
  public:
    FileSystemImpl(const Wire & wire): m_Wire(wire) {}
    int tryOpenFileToWriteOrCreate(const char* filename) override;
    bool fileExists(const char* filename) override;
    // Check whether a file is writable.
    bool fileIsWritable(const char* filename) override;
    // Truncate a file.
    bool truncateFile(const char* path) override;
    // Write a string to a file, returning true if the write was successful.
    bool writeStr(const char* filename, const char* str) override;
    // Append a string to a file, returning true if the write was successful.
    bool appendStr(const char* filename, const char* str) override;
    // Read a string from a file, returning true if the read is successful
    bool readStr(const char* filename, char* str, int max_size) override;
  private:
    const Wire & m_Wire;
    bool _writeStr(const char* filename, const char* str, int flags);
};

#endif // LTTLWHWHT_FILESYSTEM_IMPL_H