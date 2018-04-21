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

#include <map>

class AndroidSystemImpl : public AndroidSystem {
  public:
    bool has_core_services() const override;
    const std::vector<TracingCategory> & getCategories() const;
    bool tryEnableCategories(const vector<string> & categories);
    void disableAllCategories();
    void property_get_core_service_names(std::string & content) const override;
    bool setAppCmdlineProperty(const vector<string> & appNames) override;
    bool pokeBinderServices() override;
    void clearAppProperties() override;
    void log_dumping_trace() override;
    void set_errstream(FILE * errstream);
    void add_category(const char * id, const char * name, uint64_t atrace_tag);
  private:
    FILE * errstream = NULL;
    const char* k_traceTagsProperty = "debug.atrace.tags.enableflags";
    const char* k_coreServicesProp = "ro.atrace.core.services";
    const char* k_traceAppsNumberProperty = "debug.atrace.app_number";
    const char* k_traceAppsPropertyTemplate = "debug.atrace.app_%d";
    std::vector<TracingCategory> m_CategoriesList;
    std::map<std::string, TracingCategory> m_Categories;
    bool setTagsProperty(uint64_t tags);
};

#endif // LTTLWHWHT_ANDROIDSYSTEM_IMPL_H