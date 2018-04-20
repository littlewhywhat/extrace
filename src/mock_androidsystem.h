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

#ifndef LTTLWHWHT_MOCK_ANDROIDSYSTEM_H
#define LTTLWHWHT_MOCK_ANDROIDSYSTEM_H

#include <gmock/gmock.h>

#include "androidsystem.h"

class MockAndroidSystem : public AndroidSystem {
  public:
    MOCK_CONST_METHOD0(has_core_services, bool(void));
    MOCK_CONST_METHOD0(getCategories, const vector<TracingCategory> &(void));
    MOCK_METHOD1(tryEnableCategories, bool(const vector<string> &));
    MOCK_METHOD0(disableAllCategories, void(void));
    MOCK_CONST_METHOD1(property_get_core_service_names, void(string &));
    MOCK_METHOD1(setAppCmdlineProperty, bool(const vector<string> &));
    MOCK_METHOD0(pokeBinderServices, bool(void));
    MOCK_METHOD0(clearAppProperties, void(void));
    MOCK_METHOD2(compress_trace_to, void(int, int));
    MOCK_METHOD0(log_dumping_trace, void(void));
};

#endif // LTTLWHWHT_MOCK_ANDROIDSYSTEM_H