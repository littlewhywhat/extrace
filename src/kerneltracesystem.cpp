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

#include "kerneltracesystem.h"

bool KernelTraceSystem::tryEnableCategoryGroup(const vector<TraceCategoryMember> & categoryGroup) {
  for (const auto & categoryMember: categoryGroup) {
    if (!m_FTrace->tryEnableTracePoint(categoryMember.getTracePoint())
        && categoryMember.isRequired()) {
      return false;
    }
  }
  return true;
}

bool KernelTraceSystem::tryDisableCategoryGroup(const vector<TraceCategoryMember> & categoryGroup) {
  for (const auto & categoryMember: categoryGroup) {
    if (!m_FTrace->tryDisableTracePoint(categoryMember.getTracePoint())
        && categoryMember.isRequired()) {
      return false;
    }
  }
  return true;
}

bool KernelTraceSystem::tryEnableCategory(const TraceCategory & traceCategory) {
  if (supportsCategory(traceCategory)) {
    const auto & categoryGroup = m_TraceCategoryGroups[traceCategory];
    if (!tryEnableCategoryGroup(categoryGroup)) {
      fprintf(m_Wire.getErrorStream(), "error: KernelTraceSystem - can't enable category\n");
      return false;   
    }
    return true;
  }
  else if (supportsCategoryWithRoot(traceCategory)) {
    fprintf(m_Wire.getErrorStream(), "error: KernelTraceSystem - category requires root privileges.\n");
  }
  else {
    fprintf(m_Wire.getErrorStream(), "error: KernelTraceSystem - category is not supported.\n");
  }
  return false;
}

bool KernelTraceSystem::tryDisableAllCategories() {
  bool ok = false;
  for (const auto & categoryAndGroup: m_TraceCategoryGroups) {
    const auto & category = categoryAndGroup.first;
    if (supportsCategory(category)) {
      ok &= tryDisableCategoryGroup(categoryAndGroup.second);
    }
  }
  // TODO add log
  return ok;
}

bool KernelTraceSystem::trySetFunctions(const set<string> functions) {
  bool ok = true;
  if (functions.empty()) {
    return tryDisableAllFunctions();
  } else {
    ok &= m_FTrace->trySetTracer(FTrace::Tracer::FUNCTION_GRAPH);
    if (!ok) {
      fprintf(m_Wire.getErrorStream(), "error KernelTraceSystem can't set tracer to FUNCTION_GRAPH\n");
    }
    ok &= m_FTrace->tryEnableOption(FTrace::Option::FUNCGRAPH_ABSTIME);
    ok &= m_FTrace->tryEnableOption(FTrace::Option::FUNCGRAPH_CPU);
    ok &= m_FTrace->tryEnableOption(FTrace::Option::FUNCGRAPH_PROC);
    ok &= m_FTrace->tryClearFunctionFilter();
    for (const auto & func: functions) {
        ok &= m_FTrace->tryAddFunctionToFilter(func);
    }

    if (ok) {
        ok &= verifyFunctions(functions);
    }
  }
  return ok;
}

bool KernelTraceSystem::verifyFunctions(const set<string> & functions) const
{
  set<string> funcsTraced = m_FTrace->tryGetFunctionsFromFilter();
  bool ok = true;
  for (const auto & func : functions) {
    if (funcsTraced.find(func) == funcsTraced.end()) {
      fprintf(m_Wire.getErrorStream(), "error: \"%s\" is not a valid kernel function to trace.\n",
              func.c_str());
      ok = false;
    }
  }
  return ok;
}

bool KernelTraceSystem::tryDisableAllFunctions() {
  bool ok = true;
  if (m_FTrace->tracerChoiceAccessable()) {
    ok &= m_FTrace->trySetTracer(FTrace::Tracer::NOP);
    if (!ok) {
      fprintf(m_Wire.getErrorStream(), "error KernelTraceSystem can't set tracer to NOP\n");
    }
  }
  if (m_FTrace->functionFilterAccessible()) {
    ok &= m_FTrace->tryClearFunctionFilter();
    if (!ok) {
      fprintf(m_Wire.getErrorStream(), "error KernelTraceSystem can't clear function filter\n");
    }
  }
  return ok;
}

bool KernelTraceSystem::supportsCategoryWithRoot(const TraceCategory & traceCategory) const {
  for (const auto & categoryMember: m_TraceCategoryGroups.at(traceCategory)) {
    if (!m_FTrace->tracePointExists(categoryMember.getTracePoint())
        && categoryMember.isRequired()) {
      return false;
    }
  }
  return true;
}

bool KernelTraceSystem::supportsCategory(const TraceCategory & traceCategory) const {
  for (const auto & categoryMember: m_TraceCategoryGroups.at(traceCategory)) {
    if (!m_FTrace->tracePointAccessable(categoryMember.getTracePoint())
        && categoryMember.isRequired()) {
      return false;
    }
  }
  return true;
}


KernelTraceSystem::KernelTraceSystem(const Wire & wire, const shared_ptr<FTrace> & ftrace): 
                                           Wired(wire), m_FTrace(ftrace) {
  m_TraceCategoryGroups[TraceCategory::SCHED] = 
  {
    { FTrace::TracePoint::SCHED_SWITCH, true },
    { FTrace::TracePoint::SCHED_WAKEUP, true },
    { FTrace::TracePoint::SCHED_BLOCKED_REASON, true },
    { FTrace::TracePoint::SCHED_CPU_HOTPLUG, true}
  };
  m_TraceCategoryGroups[TraceCategory::IRQ] =
  {
    { FTrace::TracePoint::IRQ, true },
    { FTrace::TracePoint::IPI, false },
  };
  m_TraceCategoryGroups[TraceCategory::FREQ] =
  {
    { FTrace::TracePoint::CPU_FREQUENCY, true },
    { FTrace::TracePoint::CLOCK_SET_RATE, false },
    { FTrace::TracePoint::CPU_FREQUENCY_LIMITS, false },
  };
  m_TraceCategoryGroups[TraceCategory::MEMBUS] =
  {
    { FTrace::TracePoint::MEMORY_BUS, true },
  };
  m_TraceCategoryGroups[TraceCategory::IDLE] =
  {
    { FTrace::TracePoint::CPU_IDLE, true },
  };
  m_TraceCategoryGroups[TraceCategory::DISK] =
  {
    { FTrace::TracePoint::F2FS_SYNC_FILE_ENTER, false },
    { FTrace::TracePoint::F2FS_SYNC_FILE_EXIT, false },
    { FTrace::TracePoint::F2FS_WRITE_BEGIN, false },
    { FTrace::TracePoint::F2FS_WRITE_END, false },
    { FTrace::TracePoint::EXT4_DA_WRITE_BEGIN, false },
    { FTrace::TracePoint::EXT4_DA_WRITE_END, false },
    { FTrace::TracePoint::EXT4_SYNC_FILE_ENTER, false },
    { FTrace::TracePoint::EXT4_SYNC_FILE_EXIT, false },
    { FTrace::TracePoint::BLOCK_RQ_ISSUE, true },
    { FTrace::TracePoint::BLOCK_RQ_COMPLETE, true },
  };
  m_TraceCategoryGroups[TraceCategory::MMC] =
  {
    { FTrace::TracePoint::MMC, true },
  };
  m_TraceCategoryGroups[TraceCategory::LOAD] =
  {
    { FTrace::TracePoint::CPUFREQ_INTERACTIVE, true },
  };
  m_TraceCategoryGroups[TraceCategory::SYNC] =
  {
    { FTrace::TracePoint::SYNC, true},
  };
  m_TraceCategoryGroups[TraceCategory::WORKQUEUE] =
  {
    { FTrace::TracePoint::WORKQUEUE, true },
  };
  m_TraceCategoryGroups[TraceCategory::MEMRECLAIM] =
  {
    { FTrace::TracePoint::MM_VMSCAN_DIRECT_RECLAIM_BEGIN, true },
    { FTrace::TracePoint::MM_VMSCAN_DIRECT_RECLAIM_END, true },
    { FTrace::TracePoint::MM_VMSCAN_KSWAPD_WAKE, true },
    { FTrace::TracePoint::MM_VMSCAN_KSWAPD_SLEEP, true },
  };
  m_TraceCategoryGroups[TraceCategory::REGULATOR] =
  {
    { FTrace::TracePoint::REGULATOR, true },
  };
  m_TraceCategoryGroups[TraceCategory::BINDER_DRIVER] =
  {
    { FTrace::TracePoint::BINDER_TRANSACTION, true },
    { FTrace::TracePoint::BINDER_TRANSACTION_RECEIVED, true },
  };
  m_TraceCategoryGroups[TraceCategory::BINDER_LOCK] =
  {
    { FTrace::TracePoint::BINDER_LOCK, true },
    { FTrace::TracePoint::BINDER_LOCKED, true },
    { FTrace::TracePoint::BINDER_UNLOCK, true },
  };
  m_TraceCategoryGroups[TraceCategory::PAGECACHE] =
  {
    { FTrace::TracePoint::FILEMAP, true },
  }; 
}
