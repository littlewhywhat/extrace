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

#include "android_impl.h"

#include "utils/Trace.h"

#include <binder/IBinder.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <cutils/properties.h>
#include <utils/String8.h>

//! Number of app packages to trace
#define MAX_PACKAGES 16;

AndroidImpl::AndroidImpl(const Wire & wire): Wired(wire), Android() {
  m_TraceCategoriesTags[TraceCategory::GRAPHICS]         = ATRACE_TAG_GRAPHICS;
  m_TraceCategoriesTags[TraceCategory::INPUT]            = ATRACE_TAG_INPUT;
  m_TraceCategoriesTags[TraceCategory::VIEW]             = ATRACE_TAG_VIEW;
  m_TraceCategoriesTags[TraceCategory::WEBVIEW]          = ATRACE_TAG_WEBVIEW;
  m_TraceCategoriesTags[TraceCategory::WINDOW_MANAGER]   = ATRACE_TAG_WINDOW_MANAGER;
  m_TraceCategoriesTags[TraceCategory::ACTIVITY_MANAGER] = ATRACE_TAG_ACTIVITY_MANAGER;
  m_TraceCategoriesTags[TraceCategory::SYNC_MANAGER]     = ATRACE_TAG_SYNC_MANAGER;
  m_TraceCategoriesTags[TraceCategory::AUDIO]            = ATRACE_TAG_AUDIO;
  m_TraceCategoriesTags[TraceCategory::VIDEO]            = ATRACE_TAG_VIDEO;
  m_TraceCategoriesTags[TraceCategory::CAMERA]           = ATRACE_TAG_CAMERA;
  m_TraceCategoriesTags[TraceCategory::HAL]              = ATRACE_TAG_HAL;
  m_TraceCategoriesTags[TraceCategory::APP]              = ATRACE_TAG_APP;
  m_TraceCategoriesTags[TraceCategory::RESOURCES]        = ATRACE_TAG_RESOURCES;
  m_TraceCategoriesTags[TraceCategory::DALVIK]           = ATRACE_TAG_DALVIK;
  m_TraceCategoriesTags[TraceCategory::RS]               = ATRACE_TAG_RS;
  m_TraceCategoriesTags[TraceCategory::BIONIC]           = ATRACE_TAG_BIONIC;
  m_TraceCategoriesTags[TraceCategory::POWER]            = ATRACE_TAG_POWER;
  m_TraceCategoriesTags[TraceCategory::PACKAGE_MANAGER]  = ATRACE_TAG_PACKAGE_MANAGER;
  m_TraceCategoriesTags[TraceCategory::SYSTEM_SERVER]    = ATRACE_TAG_SYSTEM_SERVER;
  m_TraceCategoriesTags[TraceCategory::DATABASE]         = ATRACE_TAG_DATABASE;
  m_TraceCategoriesTags[TraceCategory::NETWORK]          = ATRACE_TAG_NETWORK;
}

uint32_t AndroidImpl::getTraceAppsMaxNum() const {
  return MAX_PACKAGES;
}

const string AndroidImpl::getTraceCoreServicesProperty() {
  char value[PROPERTY_VALUE_MAX];
  property_get(m_TraceCoreServicesPropertyKey, value, "");
  return value;
}

bool AndroidImpl::trySetTraceTagsProperty(uint64_t traceTags) {
  char traceTagsValue[PROPERTY_VALUE_MAX];
  snprintf(traceTagsValue, sizeof(traceTagsValue), "%#" PRIx64, traceTags);
  if (property_set(m_TraceTagsPropertyKey, traceTagsValue) < 0) {
      fprintf(m_Wire.getErrorStream(), "error AndroidImpl::trySetTraceTags\n");
      return false;
  }
  return true;
}

bool AndroidImpl::trySetTraceAppProperty(const char * appName, uint32_t id) {
  char propertyKey[PROPERTY_KEY_MAX];
  snprintf(propertyKey, sizeof(propertyKey), m_TraceAppsPropertyKeyTemplate, id);
  if (property_set(propertyKey, appName) < 0) {
    fprintf(m_Wire.getErrorStream(), "error AndroidImpl::trySetTraceAppProperty"
                                     " name: %s number: %d\n", appName, id);
    return false;
  }
  return true;
}

bool AndroidImpl::trySetTraceAppsCntProperty(uint32_t cnt) {
  char cntValue[PROPERTY_VALUE_MAX];
  snprintf(cntValue, sizeof(cntValue), "%u", cnt);
  if (property_set(m_TraceAppsCntProperty, cntValue) < 0) {
    fprintf(m_Wire.getErrorStream(), "error AndroidImpl::trySepTraceAppsCntProperty"
                                     " cnt: %u", cnt);
    return false;
  }
  return true;
}

bool AndroidImpl::tryClearTraceAppsCntProperty() {
  if (property_set(m_TraceAppsCntProperty, "") < 0) {
    fprintf(m_Wire.getErrorStream(), "error AndroidImpl::tryClearTraceAppsCntProperty");
    return false;
  }
  return true;
}

bool AndroidImpl::tryPokeBinderServices() const {
  android::sp<android::IServiceManager> sm = android::defaultServiceManager();
  android::Vector<android::String16> services = sm->listServices();
  for (size_t i = 0; i < services.size(); i++) {
      android::sp<android::IBinder> obj = sm->checkService(services[i]);
      if (obj != NULL) {
          android::Parcel data;
          if (obj->transact(android::IBinder::SYSPROPS_TRANSACTION, data,
                  NULL, 0) != android::OK) {
              if (false) {
                  // XXX: For some reason this fails on tablets trying to
                  // poke the "phone" service.  It's not clear whether some
                  // are expected to fail.
                  android::String8 svc(services[i]);
                  fprintf(m_Wire.getErrorStream(), "error poking binder service %s\n",
                      svc.string());
                  return false;
              }
          }
      }
  }
  return true;
}

void AndroidImpl::logDumpingTrace() {
  ALOGI("Dumping trace");
}

uint64_t AndroidImpl::getCategoryTraceTag(const TraceCategory & traceCategory) const {
  return m_TraceCategoriesTags.at(traceCategory); 
}
