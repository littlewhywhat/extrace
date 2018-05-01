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

#include "androidsystembuilder.h"

#include "utils/Trace.h"

AndroidSystem * AndroidSystemBuilder::build(const Wire & wire) const {
  auto * androidSystemImpl = new AndroidSystemImpl(wire);
  initDefaultCategories(androidSystemImpl);
  return androidSystemImpl;
}
#include "androidsystem_impl.h"

void AndroidSystemBuilder::initDefaultCategories(AndroidSystemImpl * androidSystemImpl) const {
  androidSystemImpl->add_category( "gfx",        "Graphics",         ATRACE_TAG_GRAPHICS         );
  androidSystemImpl->add_category( "input",      "Input",            ATRACE_TAG_INPUT            );
  androidSystemImpl->add_category( "view",       "View System",      ATRACE_TAG_VIEW             );
  androidSystemImpl->add_category( "webview",    "WebView",          ATRACE_TAG_WEBVIEW          );
  androidSystemImpl->add_category( "wm",         "Window Manager",   ATRACE_TAG_WINDOW_MANAGER   );
  androidSystemImpl->add_category( "am",         "Activity Manager", ATRACE_TAG_ACTIVITY_MANAGER );
  androidSystemImpl->add_category( "sm",         "Sync Manager",     ATRACE_TAG_SYNC_MANAGER     );
  androidSystemImpl->add_category( "audio",      "Audio",            ATRACE_TAG_AUDIO            );
  androidSystemImpl->add_category( "video",      "Video",            ATRACE_TAG_VIDEO            );
  androidSystemImpl->add_category( "camera",     "Camera",           ATRACE_TAG_CAMERA           );
  androidSystemImpl->add_category( "hal",        "Hardware Modules", ATRACE_TAG_HAL              );
  androidSystemImpl->add_category( "app",        "Application",      ATRACE_TAG_APP              );
  androidSystemImpl->add_category( "res",        "Resource Loading", ATRACE_TAG_RESOURCES        );
  androidSystemImpl->add_category( "dalvik",     "Dalvik VM",        ATRACE_TAG_DALVIK           );
  androidSystemImpl->add_category( "rs",         "RenderScript",     ATRACE_TAG_RS               );
  androidSystemImpl->add_category( "bionic",     "Bionic C Library", ATRACE_TAG_BIONIC           );
  androidSystemImpl->add_category( "power",      "Power Management", ATRACE_TAG_POWER            );
  androidSystemImpl->add_category( "pm",         "Package Manager",  ATRACE_TAG_PACKAGE_MANAGER  );
  androidSystemImpl->add_category( "ss",         "System Server",    ATRACE_TAG_SYSTEM_SERVER    );
  androidSystemImpl->add_category( "database",   "Database",         ATRACE_TAG_DATABASE         );
  androidSystemImpl->add_category( "network",    "Network",          ATRACE_TAG_NETWORK          );
}