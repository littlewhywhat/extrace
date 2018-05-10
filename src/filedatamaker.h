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

#ifndef LTTLWHWHT_FILEDATAMAKER_H
#define LTTLWHWHT_FILEDATAMAKER_H

#include "filedata.h"
#include "wire.h"

//! I am a maker of file data
class FileDataMaker {
  public:
    virtual ~FileDataMaker() {}
    //! Tries to make data from file descriptor 
    virtual FileData * tryMake(const Wire & wire, int fd) const = 0;
};

#endif // LTTLWHWHT_FILEDATAMAKER_H