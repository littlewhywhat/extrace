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

#include "kernelsystembuilder.h"

#include "androidtoolbox.h"
#include "systemtime_impl.h"
#include "enablefile.h"

void KernelSystemBuilder::initDefaultCategories(KernelSystemImpl * kernelSystemImpl) const {
  kernelSystemImpl->add_kernel_category("sched",         "CPU Scheduling",
  {
     { EnableFile::REQ, "/sys/kernel/debug/tracing/events/sched/sched_switch/enable" },
     { EnableFile::REQ, "/sys/kernel/debug/tracing/events/sched/sched_wakeup/enable" },
     { EnableFile::OPT, "/sys/kernel/debug/tracing/events/sched/sched_blocked_reason/enable" },
     { EnableFile::OPT, "/sys/kernel/debug/tracing/events/sched/sched_cpu_hotplug/enable" },
  });
  kernelSystemImpl->add_kernel_category("irq",           "IRQ Events",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/irq/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ipi/enable" },
  });
  kernelSystemImpl->add_kernel_category("freq",          "CPU Frequency",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/power/cpu_frequency/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/power/clock_set_rate/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/power/cpu_frequency_limits/enable" },
  });
  kernelSystemImpl->add_kernel_category("membus",        "Memory Bus Utilization",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/memory_bus/enable" },
  });
  kernelSystemImpl->add_kernel_category("idle",          "CPU Idle",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/power/cpu_idle/enable" },
  });
  kernelSystemImpl->add_kernel_category("disk",          "Disk I/O",
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
  kernelSystemImpl->add_kernel_category("mmc",           "eMMC commands",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/mmc/enable" },
  });
  kernelSystemImpl->add_kernel_category("load",          "CPU Load",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/cpufreq_interactive/enable" },
  });
  kernelSystemImpl->add_kernel_category("sync",          "Synchronization",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/sync/enable" },
  });
  kernelSystemImpl->add_kernel_category("workq",         "Kernel Workqueues",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/workqueue/enable" },
  });
  kernelSystemImpl->add_kernel_category("memreclaim",    "Kernel Memory Reclaim",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_begin/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_end/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_wake/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_sleep/enable" },
  });
  kernelSystemImpl->add_kernel_category("regulators",    "Voltage and Current Regulators",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/regulator/enable" },
  });
  kernelSystemImpl->add_kernel_category("binder_driver", "Binder Kernel driver",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction_received/enable" },
  });
  kernelSystemImpl->add_kernel_category("binder_lock",   "Binder global lock trace",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_lock/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_locked/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_unlock/enable" },
  });
  kernelSystemImpl->add_kernel_category("pagecache",     "Page cache",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/filemap/enable" },
  }); 
}

KernelSystem * KernelSystemBuilder::build(const Wire & wire, FileSystem * fileSystem) const {
  auto * kernelSystem = new KernelSystemImpl(wire, fileSystem,
                                             new AndroidToolBox(),
                                             new SystemTimeImpl());
  initDefaultCategories(kernelSystem);
  return kernelSystem;
}
