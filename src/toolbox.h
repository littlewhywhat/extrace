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

#ifndef LTTLWHWHT_TOOLBOX_H
#define LTTLWHWHT_TOOLBOX_H

#include <string>
#include <set>

class Toolbox {
  public:
    virtual ~Toolbox() {}
    virtual bool parseFileToTokens(const char * filename, const char * delims,
                                   std::set<std::string> & tokens) const = 0;
    virtual void parseToTokens(const char * delimseplist, const char * delims,
                               std::set<std::string> & tokens) const = 0;
};

#endif //LTTLWHWHT_TOOLBOX_H