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
#include "listsupportedcategories.h"

ListSupportedCategories::~ListSupportedCategories() {
  for (auto & category : m_KernelTraceCategories) {
    delete category.second;
  }
  for (auto & category : m_AndroidTraceCategories) {
    delete category.second;
  }
}

ListSupportedCategories::ListSupportedCategories(const Wire & wire, 
                                                 const shared_ptr<Trace> & trace):
                                                 TraceAction(wire, trace) {
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::SCHED] = 
   new KernelTraceCategory("sched", "Scheduling", KernelTraceSystem::TraceCategory::SCHED);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::IRQ] =
   new KernelTraceCategory("irq", "IRQ Events", KernelTraceSystem::TraceCategory::IRQ);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::FREQ] =
   new KernelTraceCategory("freq", "CPU Frequency", KernelTraceSystem::TraceCategory::FREQ);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::MEMBUS] =
   new KernelTraceCategory("membus", "Memory Bus Utilization", KernelTraceSystem::TraceCategory::MEMBUS);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::IDLE] =
   new KernelTraceCategory("idle", "CPU Idle", KernelTraceSystem::TraceCategory::IDLE);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::DISK] =
   new KernelTraceCategory("disk", "Disk I/O", KernelTraceSystem::TraceCategory::DISK);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::MMC] =
   new KernelTraceCategory("mmc", "eMMC commands", KernelTraceSystem::TraceCategory::MMC);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::LOAD] =
   new KernelTraceCategory("load", "CPU Load", KernelTraceSystem::TraceCategory::LOAD);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::SYNC] =
   new KernelTraceCategory("sync", "Synchronization", KernelTraceSystem::TraceCategory::SYNC);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::WORKQUEUE] =
   new KernelTraceCategory("workqueue", "Kernel Workqueues", KernelTraceSystem::TraceCategory::WORKQUEUE);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::MEMRECLAIM] =
   new KernelTraceCategory("memreclaim", "Kernel Memory Reclaim", KernelTraceSystem::TraceCategory::MEMRECLAIM);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::REGULATOR] =
   new KernelTraceCategory("regulator", "Voltage and Current Regulators", KernelTraceSystem::TraceCategory::REGULATOR);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::BINDER_DRIVER] =
   new KernelTraceCategory("binder_driver", "Binder Kernel driver", KernelTraceSystem::TraceCategory::BINDER_DRIVER);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::BINDER_LOCK] =
   new KernelTraceCategory("binder_lock", "Binder global lock trace", KernelTraceSystem::TraceCategory::BINDER_LOCK);
  m_KernelTraceCategories[KernelTraceSystem::TraceCategory::PAGECACHE] =
   new KernelTraceCategory("filemap", "Page cache", KernelTraceSystem::TraceCategory::PAGECACHE);

  m_AndroidTraceCategories[Android::TraceCategory::GRAPHICS] = 
    new AndroidTraceCategory({ "gfx", "Graphics", Android::TraceCategory::GRAPHICS });
  m_AndroidTraceCategories[Android::TraceCategory::INPUT] = 
    new AndroidTraceCategory({ "input", "Input", Android::TraceCategory::INPUT });
  m_AndroidTraceCategories[Android::TraceCategory::VIEW] = 
    new AndroidTraceCategory({ "view", "View System", Android::TraceCategory::VIEW });
  m_AndroidTraceCategories[Android::TraceCategory::WEBVIEW] = 
    new AndroidTraceCategory({ "webview", "WebView", Android::TraceCategory::WEBVIEW });
  m_AndroidTraceCategories[Android::TraceCategory::WINDOW_MANAGER] = 
    new AndroidTraceCategory({ "wm", "Window Manager", Android::TraceCategory::WINDOW_MANAGER });
  m_AndroidTraceCategories[Android::TraceCategory::ACTIVITY_MANAGER] = 
    new AndroidTraceCategory({ "am", "Activity Manager", Android::TraceCategory::ACTIVITY_MANAGER });
  m_AndroidTraceCategories[Android::TraceCategory::SYNC_MANAGER] = 
    new AndroidTraceCategory({ "sm", "Sync Manager", Android::TraceCategory::SYNC_MANAGER });
  m_AndroidTraceCategories[Android::TraceCategory::AUDIO] = 
    new AndroidTraceCategory({ "audio", "Audio", Android::TraceCategory::AUDIO });
  m_AndroidTraceCategories[Android::TraceCategory::VIDEO] = 
    new AndroidTraceCategory({ "video", "Video", Android::TraceCategory::VIDEO });
  m_AndroidTraceCategories[Android::TraceCategory::CAMERA] = 
    new AndroidTraceCategory({ "camera", "Camera", Android::TraceCategory::CAMERA });
  m_AndroidTraceCategories[Android::TraceCategory::HAL] = 
    new AndroidTraceCategory({ "hal", "Hardware Modules", Android::TraceCategory::HAL });
  m_AndroidTraceCategories[Android::TraceCategory::APP] = 
    new AndroidTraceCategory({ "app", "Application", Android::TraceCategory::APP });
  m_AndroidTraceCategories[Android::TraceCategory::RESOURCES] = 
    new AndroidTraceCategory({ "res", "Resource Loading", Android::TraceCategory::RESOURCES });
  m_AndroidTraceCategories[Android::TraceCategory::DALVIK] = 
    new AndroidTraceCategory({ "dalvik", "Dalvik VM", Android::TraceCategory::DALVIK });
  m_AndroidTraceCategories[Android::TraceCategory::RS] = 
    new AndroidTraceCategory({ "rs", "RenderScript", Android::TraceCategory::RS });
  m_AndroidTraceCategories[Android::TraceCategory::BIONIC] = 
    new AndroidTraceCategory({ "bionic", "Bionic C Library", Android::TraceCategory::BIONIC });
  m_AndroidTraceCategories[Android::TraceCategory::POWER] = 
    new AndroidTraceCategory({ "power", "Power Management", Android::TraceCategory::POWER });
  m_AndroidTraceCategories[Android::TraceCategory::PACKAGE_MANAGER] = 
    new AndroidTraceCategory({ "pm", "Package Manager", Android::TraceCategory::PACKAGE_MANAGER });
  m_AndroidTraceCategories[Android::TraceCategory::SYSTEM_SERVER] = 
    new AndroidTraceCategory({ "ss", "System Server", Android::TraceCategory::SYSTEM_SERVER });
  m_AndroidTraceCategories[Android::TraceCategory::DATABASE] = 
    new AndroidTraceCategory({ "database", "Database", Android::TraceCategory::DATABASE });
  m_AndroidTraceCategories[Android::TraceCategory::NETWORK] = 
    new AndroidTraceCategory({ "network", "Network", Android::TraceCategory::NETWORK });
}

bool ListSupportedCategories::tryRun() {
  for (const auto & entry : m_KernelTraceCategories) {
    const auto & category = entry.second;
    if (m_Trace->getKernelTraceSystem()->supportsCategory(category->getCategory())) {
      fprintf(m_Wire.getOutputStream(), "  %10s  %s\n", category->getName().c_str(),
                                                        category->getLongName().c_str());
    }
  }
  for (const auto & entry : m_AndroidTraceCategories) {
    const auto & category = entry.second;
    fprintf(m_Wire.getOutputStream(), "  %10s  %s\n", category->getName().c_str(),
                                                      category->getLongName().c_str());
  }
  return true;
}
