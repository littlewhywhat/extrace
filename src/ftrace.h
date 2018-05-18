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

#include <set>
#include <cstdint>
#include <string>

using namespace std;

class FTrace {
  public:
    enum class ClockType;
    enum class TracePoint;
    enum class Option;
    enum class Tracer;
    virtual ~FTrace() {};
    virtual int getTraceFd() = 0;
    virtual int getTracePipeFd() = 0;
    virtual bool tryCleanTrace() = 0;
    virtual bool tryAddFunctionToFilter(const string & function) = 0;
    virtual const set<string> tryGetFunctionsFromFilter() const = 0;
    virtual bool tryClearFunctionFilter() = 0;
    virtual bool functionFilterAccessible() const = 0;
    virtual bool trySetTracer(const Tracer & tracer) = 0;
    virtual bool trySetBufferSize(uint32_t bufferSize) = 0;
    virtual bool tryWriteMarker(const char * marker) = 0;
    virtual bool tracerChoiceAccessable() const = 0;
    virtual bool tracePointAccessable(const TracePoint & tracePoint) const = 0;
    virtual bool tracePointExists(const TracePoint & tracePoint) const = 0;
    virtual bool tryEnableTracePoint(const TracePoint & tracePoint) = 0;
    virtual bool tryDisableTracePoint(const TracePoint & tracePoint) = 0;
    virtual bool hasTraceClockSetTo(const ClockType & clockType) const = 0;
    virtual bool trySetClockType(const ClockType & clockType) = 0;
    virtual bool tryEnableOption(const Option & option) = 0;
    virtual bool tryDisableOption(const Option & option) = 0;
    virtual bool tryStartTrace() = 0;
    virtual bool tryStopTrace() = 0;
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
};

#endif // LTTLWHWHT_FTRACE_H