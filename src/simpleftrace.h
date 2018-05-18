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

#ifndef LTTLWHWHT_FTRACE_H
#define LTTLWHWHT_FTRACE_H

#include "wired.h"

#include <cstdint>
#include <set>
#include <map>
#include <string>
#include <memory>

#include "filesystem.h"
#include "toolbox.h"

using namespace std;

class FTrace : public Wired {
  public:
    enum class ClockType;
    enum class TracePoint;
    enum class Option;
    enum class Tracer;
    FTrace(const Wire & wire,
           shared_ptr<FileSystem> & fileSystem,
           shared_ptr<ToolBox> toolBox,
           const string & mountPoint);
    ~FTrace() {};
    int getTraceFd();
    int getTracePipeFd();
    bool tryCleanTrace();
    bool tryAddFunctionToFilter(const string & function);
    const set<string> tryGetFunctionsFromFilter() const;
    bool tryClearFunctionFilter();
    bool functionFilterAccessible() const;
    bool trySetTracer(const Tracer & tracer);
    bool trySetBufferSize(uint32_t bufferSize);
    bool tryWriteMarker(const char * marker);
    bool tracerChoiceAccessable() const;
    bool tracePointAccessable(const TracePoint & tracePoint) const;
    bool tracePointExists(const TracePoint & tracePoint) const;
    bool tryEnableTracePoint(const TracePoint & tracePoint);
    bool tryDisableTracePoint(const TracePoint & tracePoint);
    bool hasTraceClockSetTo(const ClockType & clockType) const;
    bool trySetClockType(const ClockType & clockType);
    bool tryEnableOption(const Option & option);
    bool tryDisableOption(const Option & option);
    bool tryStartTrace();
    bool tryStopTrace();
    enum class ClockType {
      GLOBAL,
      LOCAL,
      CLOCKTYPE_CNT
    };
    enum class TracePoint {
      SCHED_SWITCH,
      SCHED_WAKEUP,
      SCHED_BLOCKED_REASON,
      SCHED_CPU_HOTPLUG,
      IRQ,
      IPI,
      CPU_FREQUENCY,
      CLOCK_SET_RATE,
      CPU_FREQUENCY_LIMITS,
      MEMORY_BUS,
      CPU_IDLE,
      F2FS_SYNC_FILE_ENTER,
      F2FS_SYNC_FILE_EXIT,
      F2FS_WRITE_BEGIN,
      F2FS_WRITE_END,
      EXT4_DA_WRITE_BEGIN,
      EXT4_DA_WRITE_END,
      EXT4_SYNC_FILE_ENTER,
      EXT4_SYNC_FILE_EXIT,
      BLOCK_RQ_ISSUE,
      BLOCK_RQ_COMPLETE,
      MMC,
      CPUFREQ_INTERACTIVE,
      SYNC,
      WORKQUEUE,
      MM_VMSCAN_DIRECT_RECLAIM_BEGIN,
      MM_VMSCAN_DIRECT_RECLAIM_END,
      MM_VMSCAN_KSWAPD_WAKE,
      MM_VMSCAN_KSWAPD_SLEEP,
      REGULATOR,
      BINDER_TRANSACTION,
      BINDER_TRANSACTION_RECEIVED,
      BINDER_LOCK,
      BINDER_LOCKED,
      BINDER_UNLOCK,
      FILEMAP,
    };
    enum class Option {
      OVERWRITE,
      PRINT_TGID,
      FUNCGRAPH_ABSTIME,
      FUNCGRAPH_CPU,
      FUNCGRAPH_PROC,
    };
    enum class Tracer {
      FUNCTION_GRAPH,
      NOP,
    };
  private:
    string m_TraceClockPath;
    string m_TraceBufferSizePath;
    string m_CurrentTracerPath;
    string m_TraceFilterPath;
    string m_TracingOnPath;
    string m_TracePath;
    string m_TracePipePath;
    string m_TraceMarkerPath;
    map<TracePoint, string> m_TracePoints;
    map<Option, string> m_Options;
    map<ClockType, string> m_ClockTypes;
    map<Tracer, string> m_Tracers;
    shared_ptr<FileSystem> m_FileSystem = NULL;
    shared_ptr<ToolBox> m_ToolBox = NULL;
    bool tryConfigTracePoint(const TracePoint & tracePoint, bool enable);
    bool tryConfigFileSwitch(const char * filename, bool enable);
    // TODO transfer to FileSystem
    int tryOpenFile(const char * filename);
};

#endif // LTTLWHWHT_FTRACE_H