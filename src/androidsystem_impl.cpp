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

#include "androidsystem_impl.h"

#include "android.h"

AndroidSystemImpl::AndroidSystemImpl(const Wire & wire, Android * android):
                                     m_Wire(wire), m_Android(android) {
  add_category("gfx",        "Graphics",         android->getCategoryTraceTag(Android::TraceCategory::GRAPHICS)         );
  add_category("input",      "Input",            android->getCategoryTraceTag(Android::TraceCategory::INPUT)            );
  add_category("view",       "View System",      android->getCategoryTraceTag(Android::TraceCategory::VIEW)             );
  add_category("webview",    "WebView",          android->getCategoryTraceTag(Android::TraceCategory::WEBVIEW)          );
  add_category("wm",         "Window Manager",   android->getCategoryTraceTag(Android::TraceCategory::WINDOW_MANAGER)   );
  add_category("am",         "Activity Manager", android->getCategoryTraceTag(Android::TraceCategory::ACTIVITY_MANAGER) );
  add_category("sm",         "Sync Manager",     android->getCategoryTraceTag(Android::TraceCategory::SYNC_MANAGER)     );
  add_category("audio",      "Audio",            android->getCategoryTraceTag(Android::TraceCategory::AUDIO)            );
  add_category("video",      "Video",            android->getCategoryTraceTag(Android::TraceCategory::VIDEO)            );
  add_category("camera",     "Camera",           android->getCategoryTraceTag(Android::TraceCategory::CAMERA)           );
  add_category("hal",        "Hardware Modules", android->getCategoryTraceTag(Android::TraceCategory::HAL)              );
  add_category("app",        "Application",      android->getCategoryTraceTag(Android::TraceCategory::APP)              );
  add_category("res",        "Resource Loading", android->getCategoryTraceTag(Android::TraceCategory::RESOURCES)        );
  add_category("dalvik",     "Dalvik VM",        android->getCategoryTraceTag(Android::TraceCategory::DALVIK)           );
  add_category("rs",         "RenderScript",     android->getCategoryTraceTag(Android::TraceCategory::RS)               );
  add_category("bionic",     "Bionic C Library", android->getCategoryTraceTag(Android::TraceCategory::BIONIC)           );
  add_category("power",      "Power Management", android->getCategoryTraceTag(Android::TraceCategory::POWER)            );
  add_category("pm",         "Package Manager",  android->getCategoryTraceTag(Android::TraceCategory::PACKAGE_MANAGER)  );
  add_category("ss",         "System Server",    android->getCategoryTraceTag(Android::TraceCategory::SYSTEM_SERVER)    );
  add_category("database",   "Database",         android->getCategoryTraceTag(Android::TraceCategory::DATABASE)         );
  add_category("network",    "Network",          android->getCategoryTraceTag(Android::TraceCategory::NETWORK)          );
}

void AndroidSystemImpl::add_category(const char * id, const char * name, uint64_t atrace_tag) {
  m_Categories[id] = { id, name, atrace_tag, {}, false };
  m_CategoriesList.push_back({ id, name, atrace_tag, {}, false });
}

bool AndroidSystemImpl::has_core_services() const {
  const string coreServicesPropertyValue = m_Android->getTraceCoreServicesProperty();
  return !coreServicesPropertyValue.empty();
}

void AndroidSystemImpl::property_get_core_service_names(std::string & content) const {
  const string coreServicesPropertyValue = m_Android->getTraceCoreServicesProperty();
  content += coreServicesPropertyValue;
}

bool AndroidSystemImpl::setAppCmdlineProperty(const vector<string> & appNames) {
  if (appNames.size() > m_Android->getTraceAppsMaxNum()) {
    fprintf(m_Wire.getErrorStream(), "error AndroidSystemImpl::setAppCmdlineProperty"
                                     " too many apps to trace: more than %d\n",
                                     m_Android->getTraceAppsMaxNum());
    clearAppProperties();
    return false;
  }
  for (size_t i = 0; i < appNames.size(); i++) {
    if (!m_Android->trySetTraceAppProperty(appNames[i].c_str(), i)) {
      fprintf(m_Wire.getErrorStream(), "error AndroidSystemImpl::setAppCmdlineProperty set appname\n");
      clearAppProperties();
      return false;
    }
  }
  if (!m_Android->trySetTraceAppsCntProperty(appNames.size())) {
    fprintf(m_Wire.getErrorStream(), "error AndroidSystemImpl::setAppCmdlineProperty set cnt\n");
    clearAppProperties();
    return false;
  }
  return true;
}

bool AndroidSystemImpl::pokeBinderServices() {
  return m_Android->tryPokeBinderServices();
}

bool AndroidSystemImpl::setTagsProperty(uint64_t tags) {
  if (!m_Android->trySetTraceTagsProperty(tags)) {
    fprintf(m_Wire.getErrorStream(), "error setting trace tags system property\n");
    return false;
  }
  return true;
}

void AndroidSystemImpl::clearAppProperties() {
  for (uint32_t i = 0; i < m_Android->getTraceAppsMaxNum(); i++) {
    if (!m_Android->trySetTraceAppProperty("", i)) {
      fprintf(m_Wire.getErrorStream(), "error clearing app properties\n");
    }
  }
  if (!m_Android->tryClearTraceAppsCntProperty()) {
    fprintf(m_Wire.getErrorStream(), "error clearing app properties\n");
  }
}

void AndroidSystemImpl::log_dumping_trace() {
  m_Android->logDumpingTrace();
}

const std::vector<TracingCategory> & AndroidSystemImpl::getCategories() const {
  return m_CategoriesList;
}

bool AndroidSystemImpl::tryEnableCategories(const vector<string> & categories) {
    uint64_t tags = 0;
    for (const auto & id : categories) {
        if (m_Categories.find(id) == m_Categories.end()) {
            fprintf(m_Wire.getErrorStream(), "category is not supported - %s", id.c_str());
            return false;
        }
        tags |= m_Categories[id].tags;
    }
    return setTagsProperty(tags);
}

void AndroidSystemImpl::disableAllCategories() {
  setTagsProperty(0);
}
