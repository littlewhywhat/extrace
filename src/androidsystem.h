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

#ifndef LTTLWHWHT_ANDROIDSYSTEM_H
#define LTTLWHWHT_ANDROIDSYSTEM_H

#include "tracingcategory.h"

#include <string>
#include <inttypes.h>
#include <vector>

using namespace std;

class AndroidSystem {
  public:
    virtual ~AndroidSystem() {}
    virtual bool has_core_services() const = 0;
    // virtual bool setCategoriesEnableFromFile(const char * filename) = 0;
    virtual const vector<TracingCategory> & getCategories() const = 0;
    virtual bool tryEnableCategories(const vector<string> & categories) = 0;
    virtual void disableAllCategories() = 0;
    virtual void property_get_core_service_names(string & content) const = 0;
    virtual bool setAppCmdlineProperty(const vector<string> & appNames) = 0;
    virtual bool pokeBinderServices() = 0;
    virtual void clearAppProperties() = 0;
    virtual void compress_trace_to(int traceFD, int outFd) = 0;
    virtual void log_dumping_trace() = 0;
};

#endif // LTTLWHWHT_ANDROIDSYSTEM_H