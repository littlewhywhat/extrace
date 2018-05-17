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

#ifndef LTTLWHWHT_SIMPLEANDROIDSYSTEM_H
#define LTTLWHWHT_SIMPLEANDROIDSYSTEM_H

#include "androidtracesystem.h"
#include "toolbox.h"

#include <set>
#include <memory>

using namespace std;

//! I am an android trace system that simply does what it should

//! I can do everything that android trace system does
class SimpleAndroidTraceSystem : public AndroidTraceSystem {
  public:
    //! Constructs me for Android
    SimpleAndroidTraceSystem(const Wire & wire,
                             const shared_ptr<Android> & android,
                             const shared_ptr<ToolBox> & toolBox):
                             AndroidTraceSystem(wire),
                             m_Android(android),
                             myToolBox(toolBox) {}
    bool canTraceCoreServices() const override;
    void rememberToTrace(const string & appName) override;
    void rememberToTrace(const Android::TraceCategory & category) override;
    void rememberToTraceCoreServices() override;
    bool tryToTrace() override;
    void forgetAll() override;
    bool tryNotToTrace() override;
  private:
    shared_ptr<Android> m_Android = NULL;
    shared_ptr<ToolBox> myToolBox = NULL;
    set<string> m_AppNames;
    set<Android::TraceCategory> m_Categories;
    void stopTraceAllApps();
    bool tryTraceCategories();
    bool tryTraceApps();
};

#endif // LTTLWHWHT_SIMPLEANDROIDSYSTEM_H