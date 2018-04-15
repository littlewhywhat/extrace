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

#define LOG_TAG "atrace"

#include "filesystem_impl.h"
#include "systemtime_impl.h"
#include "androidsystem_impl.h"
#include "kernelsystem_impl.h"
#include "atraceapp.h"

#include <signal.h>      // sigaction
#include <utils/Trace.h> // ATRACE_TAGs

AtraceApp atrace;

void handleSignal(int /*signo*/)
{
    atrace.handleSignal();
}

void registerSigHandler()
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handleSignal;
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

int main(int argc, char ** argv) {
  FileSystemImpl * file_system = new FileSystemImpl();
  KernelSystemImpl * kernel_system_impl = new KernelSystemImpl();
  kernel_system_impl->set_errstream(stderr);
  kernel_system_impl->set_file_system(file_system);
  AndroidSystemImpl * android_system_impl = new AndroidSystemImpl();
  android_system_impl->set_errstream(stderr);

  atrace.set_systime(new SystemTimeImpl());
  atrace.set_kernel_system(kernel_system_impl);
  atrace.set_android_system(android_system_impl);
  atrace.set_errstream(stderr);
  atrace.set_outstream(stdout);
  atrace.add_android_category( "gfx",        "Graphics",         ATRACE_TAG_GRAPHICS         );
  atrace.add_android_category( "input",      "Input",            ATRACE_TAG_INPUT            );
  atrace.add_android_category( "view",       "View System",      ATRACE_TAG_VIEW             );
  atrace.add_android_category( "webview",    "WebView",          ATRACE_TAG_WEBVIEW          );
  atrace.add_android_category( "wm",         "Window Manager",   ATRACE_TAG_WINDOW_MANAGER   );
  atrace.add_android_category( "am",         "Activity Manager", ATRACE_TAG_ACTIVITY_MANAGER );
  atrace.add_android_category( "sm",         "Sync Manager",     ATRACE_TAG_SYNC_MANAGER     );
  atrace.add_android_category( "audio",      "Audio",            ATRACE_TAG_AUDIO            );
  atrace.add_android_category( "video",      "Video",            ATRACE_TAG_VIDEO            );
  atrace.add_android_category( "camera",     "Camera",           ATRACE_TAG_CAMERA           );
  atrace.add_android_category( "hal",        "Hardware Modules", ATRACE_TAG_HAL              );
  atrace.add_android_category( "app",        "Application",      ATRACE_TAG_APP              );
  atrace.add_android_category( "res",        "Resource Loading", ATRACE_TAG_RESOURCES        );
  atrace.add_android_category( "dalvik",     "Dalvik VM",        ATRACE_TAG_DALVIK           );
  atrace.add_android_category( "rs",         "RenderScript",     ATRACE_TAG_RS               );
  atrace.add_android_category( "bionic",     "Bionic C Library", ATRACE_TAG_BIONIC           );
  atrace.add_android_category( "power",      "Power Management", ATRACE_TAG_POWER            );
  atrace.add_android_category( "pm",         "Package Manager",  ATRACE_TAG_PACKAGE_MANAGER  );
  atrace.add_android_category( "ss",         "System Server",    ATRACE_TAG_SYSTEM_SERVER    );
  atrace.add_android_category( "database",   "Database",         ATRACE_TAG_DATABASE         );
  atrace.add_android_category( "network",    "Network",          ATRACE_TAG_NETWORK          );

  atrace.set_android_core_services("core_services", "Core Services");

  atrace.add_kernel_category("sched",         "CPU Scheduling",
  {
     { EnableFile::REQ, "/sys/kernel/debug/tracing/events/sched/sched_switch/enable" },
     { EnableFile::REQ, "/sys/kernel/debug/tracing/events/sched/sched_wakeup/enable" },
     { EnableFile::OPT, "/sys/kernel/debug/tracing/events/sched/sched_blocked_reason/enable" },
     { EnableFile::OPT, "/sys/kernel/debug/tracing/events/sched/sched_cpu_hotplug/enable" },
  });
  atrace.add_kernel_category("irq",           "IRQ Events",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/irq/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ipi/enable" },
  });
  atrace.add_kernel_category("freq",          "CPU Frequency",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/power/cpu_frequency/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/power/clock_set_rate/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/power/cpu_frequency_limits/enable" },
  });
  atrace.add_kernel_category("membus",        "Memory Bus Utilization",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/memory_bus/enable" },
  });
  atrace.add_kernel_category("idle",          "CPU Idle",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/power/cpu_idle/enable" },
  });
  atrace.add_kernel_category("disk",          "Disk I/O",
  {
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_sync_file_enter/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_sync_file_exit/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_write_begin/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_write_end/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_da_write_begin/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_da_write_end/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_sync_file_enter/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_sync_file_exit/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/block/block_rq_issue/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/block/block_rq_complete/enable" },
  });
  atrace.add_kernel_category("mmc",           "eMMC commands",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/mmc/enable" },
  });
  atrace.add_kernel_category("load",          "CPU Load",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/cpufreq_interactive/enable" },
  });
  atrace.add_kernel_category("sync",          "Synchronization",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/sync/enable" },
  });
  atrace.add_kernel_category("workq",         "Kernel Workqueues",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/workqueue/enable" },
  });
  atrace.add_kernel_category("memreclaim",    "Kernel Memory Reclaim",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_begin/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_end/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_wake/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_sleep/enable" },
  });
  atrace.add_kernel_category("regulators",    "Voltage and Current Regulators",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/regulator/enable" },
  });
  atrace.add_kernel_category("binder_driver", "Binder Kernel driver",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction_received/enable" },
  });
  atrace.add_kernel_category("binder_lock",   "Binder global lock trace",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_lock/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_locked/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_unlock/enable" },
  });
  atrace.add_kernel_category("pagecache",     "Page cache",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/filemap/enable" },
  });
  registerSigHandler();
  int res = atrace.run_atrace(argc, argv);
  return res;
}
