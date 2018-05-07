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

#ifndef LTTLWHWHT_ANDROIDTRACECATEGORY_H
#define LTTLWHWHT_ANDROIDTRACECATEGORY_H

#include "androidtracecategory.h"

#include <string>
#include <stdint.h>

using namespace std;

class AndroidTraceCategory {
  public:
    AndroidTraceCategory() {}
    AndroidTraceCategory(const char * name, const char * longName,
                         const uint64_t & tag): m_Name(name), m_LongName(longName),
                         m_Tag(tag) {}
    const string & getName() const;
    const string & getLongName() const;
    const uint64_t & getTag() const;
  private:
    string m_Name;
    string m_LongName;
    uint64_t m_Tag; 
};

#endif // LTTLWHWHT_ANDROIDTRACECATEGORY_H