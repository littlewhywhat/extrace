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

#ifndef LTTLWHWHT_ANDROIDSYSTEM_IMPL_H
#define LTTLWHWHT_ANDROIDSYSTEM_IMPL_H

#include "androidsystem.h"
#include "tracingcategory.h"
#include "wire.h"
#include "android.h"

#include <map>
#include <vector>

using namespace std;

class AndroidSystemImpl : public AndroidSystem {
  public:
    AndroidSystemImpl(const Wire & wire, Android * android);
    bool has_core_services() const override;
    const vector<TracingCategory> & getCategories() const override;
    bool tryEnableCategories(const vector<string> & categories) override;
    void disableAllCategories() override;
    void property_get_core_service_names(string & content) const override;
    bool setAppCmdlineProperty(const vector<string> & appNames) override;
    bool pokeBinderServices() override;
    void clearAppProperties() override;
    void log_dumping_trace() override;
    void add_category(const char * id, const char * name, uint64_t atrace_tag);
  private:
    const Wire & m_Wire;
    Android * m_Android = NULL;
    vector<TracingCategory> m_CategoriesList;
    map<string, TracingCategory> m_Categories;
    bool setTagsProperty(uint64_t tags);
};

#endif // LTTLWHWHT_ANDROIDSYSTEM_IMPL_H