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

#include "android.h"

#include <string>

using namespace std;

class AndroidTraceCategory {
  public:
    AndroidTraceCategory(const string & name, const string & longName,
                         const Android::TraceCategory & category):
                         m_Name(name), m_LongName(longName), m_Category(category) {}
    const string & getName() const;
    const string & getLongName() const;
    const Android::TraceCategory & getCategory() const;
  private:
    string m_Name;
    string m_LongName;
    Android::TraceCategory m_Category;
};

#endif // LTTLWHWHT_ANDROIDTRACECATEGORY_H