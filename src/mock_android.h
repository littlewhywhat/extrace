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

#ifndef LTTLWHWHT_MOCK_ANDROID_H
#define LTTLWHWHT_MOCK_ANDROID_H

#include <gmock/gmock.h>

#include "android.h"

class MockAndroid : public Android {
  public:
    MOCK_CONST_METHOD0(getTraceAppsMaxNum, uint32_t());
    MOCK_METHOD0(getTraceCoreServicesProperty, const string());
    MOCK_METHOD1(trySetTraceTagsProperty, bool(uint64_t traceTags));
    MOCK_METHOD2(trySetTraceAppProperty, bool(const char * appName, uint32_t id));
    MOCK_METHOD1(trySetTraceAppsCntProperty, bool(uint32_t cnt));
    MOCK_METHOD0(tryClearTraceAppsCntProperty, bool());
    MOCK_CONST_METHOD0(tryPokeBinderServices, bool());
    MOCK_METHOD0(logDumpingTrace, void());
    MOCK_CONST_METHOD1(getCategoryTraceTag, uint64_t(const TraceCategory & traceCategory));
};
#endif // LTTLWHWHT_MOCK_ANDROID_H