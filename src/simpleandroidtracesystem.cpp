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

#include "simpleandroidtracesystem.h"

#include "android.h"

bool SimpleAndroidTraceSystem::canTraceCoreServices() const {
  const string coreServicesPropertyValue = m_Android->getTraceCoreServicesProperty();
  return !coreServicesPropertyValue.empty();
}

void SimpleAndroidTraceSystem::rememberToTrace(const string & appName) {
  m_AppNames.insert(appName);
}

void SimpleAndroidTraceSystem::rememberToTrace(const Android::TraceCategory & category) {
  m_Categories.insert(category);
}

void SimpleAndroidTraceSystem::rememberToTraceCoreServices() {
  const string coreServicesPropertyValue = m_Android->getTraceCoreServicesProperty();
  set<string> tokens;
  myToolBox->parseToTokens(coreServicesPropertyValue.c_str(), ",", tokens);
  for (const auto & token : tokens) {
    rememberToTrace(token);
  }
}

void SimpleAndroidTraceSystem::forgetAll() {
  m_AppNames.clear();
  m_Categories.clear();
}

bool SimpleAndroidTraceSystem::tryToTrace() {
  bool ok = true;
  ok &= tryTraceApps();
  ok &= tryTraceCategories();
  m_Android->logDumpingTrace();
  ok &= m_Android->tryPokeBinderServices();
  return ok;
}

bool SimpleAndroidTraceSystem::tryNotToTrace() {
  bool ok = true;
  stopTraceAllApps();
  ok &= m_Android->trySetTraceTagsProperty(0);
  ok &= m_Android->tryPokeBinderServices();
  return ok;
}

bool SimpleAndroidTraceSystem::tryTraceApps() {
  if (m_AppNames.size() > m_Android->getTraceAppsMaxNum()) {
    fprintf(m_Wire.getErrorStream(), "error SimpleAndroidTraceSystem::setAppCmdlineProperty"
                                     " too many apps to trace: more than %d\n",
                                     m_Android->getTraceAppsMaxNum());
    stopTraceAllApps();
    return false;
  }
  int i = 0;
  for (const auto & appName : m_AppNames) {
    if (!m_Android->trySetTraceAppProperty(appName.c_str(), i)) {
      fprintf(m_Wire.getErrorStream(), "error SimpleAndroidTraceSystem::setAppCmdlineProperty set appname\n");
      stopTraceAllApps();
      return false;
    }
    i++;
  }
  if (!m_Android->trySetTraceAppsCntProperty(m_AppNames.size())) {
    fprintf(m_Wire.getErrorStream(), "error SimpleAndroidTraceSystem::setAppCmdlineProperty set cnt\n");
    stopTraceAllApps();
    return false;
  }
  return true;
}

bool SimpleAndroidTraceSystem::tryTraceCategories() {
  uint64_t tags = 0;
  for (const auto & category : m_Categories) {
    tags |= m_Android->getCategoryTraceTag(category);
  }
  if (!m_Android->trySetTraceTagsProperty(tags)) {
    fprintf(m_Wire.getErrorStream(), "error trying trace categories\n");
    return false;
  }
  return true;
}

void SimpleAndroidTraceSystem::stopTraceAllApps() {
  for (uint32_t i = 0; i < m_Android->getTraceAppsMaxNum(); i++) {
    if (!m_Android->trySetTraceAppProperty("", i)) {
      fprintf(m_Wire.getErrorStream(), "error clearing app properties\n");
    }
  }
  if (!m_Android->tryClearTraceAppsCntProperty()) {
    fprintf(m_Wire.getErrorStream(), "error clearing app properties\n");
  }
}
