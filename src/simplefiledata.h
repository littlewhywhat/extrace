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

#ifndef LTTLWHWHT_SIMPLE_FILEDATA_H
#define LTTLWHWHT_SIMPLE_FILEDATA_H

#include "filedata.h"

//! I am a data of file descriptor that can simply send herself to another file descriptor
class SimpleFileData : public FileData {
  public:
    //! Creates me from regular file descriptor
    SimpleFileData(const Wire & wire, int fd): FileData(wire, fd) {}
    //! Sends me to another file descriptor 
    bool trySendTo(int fd) const override;
};

#endif // LTTLWHWHT_SIMPLE_FILEDATA_H