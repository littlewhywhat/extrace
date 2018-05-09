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
#include "ftrace.h"

void KernelSystemBuilder::initDefaultCategories(KernelSystemImpl * kernelSystemImpl) const {
  kernelSystemImpl->add_kernel_category("sched", "CPU Scheduling",
  {
     { EnableFile::REQ, FTrace::TracePoint::SCHED_SWITCH },
     { EnableFile::REQ, FTrace::TracePoint::SCHED_WAKEUP },
     { EnableFile::OPT, FTrace::TracePoint::SCHED_BLOCKED_REASON },
     { EnableFile::OPT, FTrace::TracePoint::SCHED_CPU_HOTPLUG },
  });
  kernelSystemImpl->add_kernel_category("irq", "IRQ Events",
  {
     { EnableFile::REQ, FTrace::TracePoint::IRQ },
     { EnableFile::OPT, FTrace::TracePoint::IPI },
  });
  kernelSystemImpl->add_kernel_category("freq", "CPU Frequency",
  {
     { EnableFile::REQ, FTrace::TracePoint::CPU_FREQUENCY },
     { EnableFile::OPT, FTrace::TracePoint::CLOCK_SET_RATE },
     { EnableFile::OPT, FTrace::TracePoint::CPU_FREQUENCY_LIMITS },
  });
  kernelSystemImpl->add_kernel_category("membus", "Memory Bus Utilization",
  {
     { EnableFile::REQ, FTrace::TracePoint::MEMORY_BUS },
  });
  kernelSystemImpl->add_kernel_category("idle", "CPU Idle",
  {
     { EnableFile::REQ, FTrace::TracePoint::CPU_IDLE },
  });
  kernelSystemImpl->add_kernel_category("disk", "Disk I/O",
  {
     { EnableFile::OPT, FTrace::TracePoint::F2FS_SYNC_FILE_ENTER },
     { EnableFile::OPT, FTrace::TracePoint::F2FS_SYNC_FILE_EXIT },
     { EnableFile::OPT, FTrace::TracePoint::F2FS_WRITE_BEGIN },
     { EnableFile::OPT, FTrace::TracePoint::F2FS_WRITE_END },
     { EnableFile::OPT, FTrace::TracePoint::EXT4_DA_WRITE_BEGIN },
     { EnableFile::OPT, FTrace::TracePoint::EXT4_DA_WRITE_END },
     { EnableFile::OPT, FTrace::TracePoint::EXT4_SYNC_FILE_ENTER },
     { EnableFile::OPT, FTrace::TracePoint::EXT4_SYNC_FILE_EXIT },
     { EnableFile::REQ, FTrace::TracePoint::BLOCK_RQ_ISSUE },
     { EnableFile::REQ, FTrace::TracePoint::BLOCK_RQ_COMPLETE },
  });
  kernelSystemImpl->add_kernel_category("mmc", "eMMC commands",
  {
     { EnableFile::REQ, FTrace::TracePoint::MMC },
  });
  kernelSystemImpl->add_kernel_category("load", "CPU Load",
  {
     { EnableFile::REQ, FTrace::TracePoint::CPUFREQ_INTERACTIVE },
  });
  kernelSystemImpl->add_kernel_category("sync", "Synchronization",
  {
     { EnableFile::REQ, FTrace::TracePoint::SYNC},
  });
  kernelSystemImpl->add_kernel_category("workq", "Kernel Workqueues",
  {
     { EnableFile::REQ, FTrace::TracePoint::WORKQUEUE },
  });
  kernelSystemImpl->add_kernel_category("memreclaim", "Kernel Memory Reclaim",
  {
     { EnableFile::REQ, FTrace::TracePoint::MM_VMSCAN_DIRECT_RECLAIM_BEGIN },
     { EnableFile::REQ, FTrace::TracePoint::MM_VMSCAN_DIRECT_RECLAIM_END },
     { EnableFile::REQ, FTrace::TracePoint::MM_VMSCAN_KSWAPD_WAKE },
     { EnableFile::REQ, FTrace::TracePoint::MM_VMSCAN_KSWAPD_SLEEP },
  });
  kernelSystemImpl->add_kernel_category("regulators", "Voltage and Current Regulators",
  {
     { EnableFile::REQ, FTrace::TracePoint::REGULATOR },
  });
  kernelSystemImpl->add_kernel_category("binder_driver", "Binder Kernel driver",
  {
     { EnableFile::REQ, FTrace::TracePoint::BINDER_TRANSACTION },
     { EnableFile::REQ, FTrace::TracePoint::BINDER_TRANSACTION_RECEIVED },
  });
  kernelSystemImpl->add_kernel_category("binder_lock", "Binder global lock trace",
  {
     { EnableFile::REQ, FTrace::TracePoint::BINDER_LOCK },
     { EnableFile::REQ, FTrace::TracePoint::BINDER_LOCKED },
     { EnableFile::REQ, FTrace::TracePoint::BINDER_UNLOCK },
  });
  kernelSystemImpl->add_kernel_category("pagecache", "Page cache",
  {
     { EnableFile::REQ, FTrace::TracePoint::FILEMAP },
  }); 
}

KernelSystem * KernelSystemBuilder::build(const Wire & wire, FileSystem * fileSystem) const {
  auto * kernelSystem = new KernelSystemImpl(wire, fileSystem,
                                             new AndroidToolBox(),
                                             new SystemTimeImpl(),
                                             new FTrace(wire, fileSystem, new AndroidToolBox()));
  initDefaultCategories(kernelSystem);
  return kernelSystem;
}
