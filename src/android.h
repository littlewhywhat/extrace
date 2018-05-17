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

#ifndef LTTLWHWHT_ANDROID_H
#define LTTLWHWHT_ANDROID_H

#include "wired.h"

#include <string>
#include <vector>
#include <stdint.h>

using namespace std;

class Android {
  public:
    enum class TraceCategory;
    virtual ~Android() {}
    virtual uint32_t getTraceAppsMaxNum() const = 0;
    virtual const string getTraceCoreServicesProperty() = 0;
    virtual bool trySetTraceTagsProperty(uint64_t traceTags) = 0;
    virtual bool trySetTraceAppProperty(const char * appName, uint32_t id) = 0;
    virtual bool trySetTraceAppsCntProperty(uint32_t cnt) = 0;
    virtual bool tryClearTraceAppsCntProperty() = 0;
    virtual bool tryPokeBinderServices() const = 0;
    virtual void logDumpingTrace() = 0;
    virtual uint64_t getCategoryTraceTag(const TraceCategory & traceCategory) const = 0;
    enum class TraceCategory {
      GRAPHICS,
      INPUT,
      VIEW,
      WEBVIEW,
      WINDOW_MANAGER,
      ACTIVITY_MANAGER,
      SYNC_MANAGER,
      AUDIO,
      VIDEO,
      CAMERA,
      HAL,
      APP,
      RESOURCES,
      DALVIK,
      RS,
      BIONIC,
      POWER,
      PACKAGE_MANAGER,
      SYSTEM_SERVER,
      DATABASE,
      NETWORK,
    };
};

#endif // LTTLWHWHT_ANDROID_H