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

#ifndef LTTLWHWHT_ANDROID_IMPL_H
#define LTTLWHWHT_ANDROID_IMPL_H

#include "android.h"

using namespace std;

class AndroidImpl : public Android {
  public:
    AndroidImpl(const Wire & wire);
    ~AndroidImpl() {}
    uint32_t getTraceAppsMaxNum() const override;
    const string getTraceCoreServicesProperty() override;
    bool trySetTraceTagsProperty(uint64_t traceTags) override;
    bool trySetTraceAppProperty(const char * appName, uint32_t id) override;
    bool trySetTraceAppsCntProperty(uint32_t cnt) override;
    bool tryClearTraceAppsCntProperty() override;
    bool tryPokeBinderServices() const override;
    void logDumpingTrace() override;
    uint64_t getCategoryTraceTag(TraceCategory traceCategory) const override;
  private:
    uint64_t m_TraceCategoriesTags[TRACE_CATEGORIES_NUM];
    const char* m_TraceTagsPropertyKey = "debug.atrace.tags.enableflags";
    const char* m_TraceCoreServicesPropertyKey = "ro.atrace.core.services";
    const char* m_TraceAppsCntProperty = "debug.atrace.app_number";
    const char* m_TraceAppsPropertyKeyTemplate = "debug.atrace.app_%d";
};

#endif // LTTLWHWHT_ANDROID_IMPL_H