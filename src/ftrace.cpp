#include "ftrace.h"

#include <errno.h>  // errno
#include <fcntl.h>  // creat, ope, O_WRONLY, O_CREAT
#include <string.h>

FTrace::FTrace(const Wire & wire,
               shared_ptr<FileSystem> & fileSystem,
               shared_ptr<ToolBox> toolBox,
               const string & mountPoint):
                      Wired(wire), m_FileSystem(fileSystem), m_ToolBox(toolBox)
{
  m_TraceClockPath =  mountPoint + "/tracing/trace_clock";
  m_TraceBufferSizePath =  mountPoint + "/tracing/buffer_size_kb";
  m_CurrentTracerPath =  mountPoint + "/tracing/current_tracer";
  m_TraceFilterPath =  mountPoint + "/tracing/set_ftrace_filter";
  m_TracingOnPath =  mountPoint + "/tracing/tracing_on";
  m_TracePath =  mountPoint + "/tracing/trace";
  m_TracePipePath =  mountPoint + "/tracing/trace_pipe";
  m_TraceMarkerPath =  mountPoint + "/tracing/trace_marker";
  m_TracePoints[TracePoint::SCHED_SWITCH] = mountPoint + "/tracing/events/sched/sched_switch/enable";
  m_TracePoints[TracePoint::SCHED_WAKEUP] = mountPoint + "/tracing/events/sched/sched_wakeup/enable";
  m_TracePoints[TracePoint::SCHED_BLOCKED_REASON] = mountPoint + "/tracing/events/sched/sched_blocked_reason/enable";
  m_TracePoints[TracePoint::SCHED_CPU_HOTPLUG] = mountPoint + "/tracing/events/sched/sched_cpu_hotplug/enable";
  m_TracePoints[TracePoint::IRQ] = mountPoint + "/tracing/events/irq/enable";
  m_TracePoints[TracePoint::IPI] = mountPoint + "/tracing/events/ipi/enable";
  m_TracePoints[TracePoint::CPU_FREQUENCY] = mountPoint + "/tracing/events/power/cpu_frequency/enable";
  m_TracePoints[TracePoint::CLOCK_SET_RATE] = mountPoint + "/tracing/events/power/clock_set_rate/enable";
  m_TracePoints[TracePoint::CPU_FREQUENCY_LIMITS] = mountPoint + "/tracing/events/power/cpu_frequency_limits/enable";
  m_TracePoints[TracePoint::MEMORY_BUS] = mountPoint + "/tracing/events/memory_bus/enable";
  m_TracePoints[TracePoint::CPU_IDLE] = mountPoint + "/tracing/events/power/cpu_idle/enable";
  m_TracePoints[TracePoint::F2FS_SYNC_FILE_ENTER] = mountPoint + "/tracing/events/f2fs/f2fs_sync_file_enter/enable";
  m_TracePoints[TracePoint::F2FS_SYNC_FILE_EXIT] = mountPoint + "/tracing/events/f2fs/f2fs_sync_file_exit/enable";
  m_TracePoints[TracePoint::F2FS_WRITE_BEGIN] = mountPoint + "/tracing/events/f2fs/f2fs_write_begin/enable";
  m_TracePoints[TracePoint::F2FS_WRITE_END] = mountPoint + "/tracing/events/f2fs/f2fs_write_end/enable";
  m_TracePoints[TracePoint::EXT4_DA_WRITE_BEGIN] = mountPoint + "/tracing/events/ext4/ext4_da_write_begin/enable";
  m_TracePoints[TracePoint::EXT4_DA_WRITE_END] = mountPoint + "/tracing/events/ext4/ext4_da_write_end/enable";
  m_TracePoints[TracePoint::EXT4_SYNC_FILE_ENTER] = mountPoint + "/tracing/events/ext4/ext4_sync_file_enter/enable";
  m_TracePoints[TracePoint::EXT4_SYNC_FILE_EXIT] = mountPoint + "/tracing/events/ext4/ext4_sync_file_exit/enable";
  m_TracePoints[TracePoint::BLOCK_RQ_ISSUE] = mountPoint + "/tracing/events/block/block_rq_issue/enable";
  m_TracePoints[TracePoint::BLOCK_RQ_COMPLETE] = mountPoint + "/tracing/events/block/block_rq_complete/enable";
  m_TracePoints[TracePoint::MMC] = mountPoint + "/tracing/events/mmc/enable";
  m_TracePoints[TracePoint::CPUFREQ_INTERACTIVE] = mountPoint + "/tracing/events/cpufreq_interactive/enable";
  m_TracePoints[TracePoint::SYNC] = mountPoint + "/tracing/events/sync/enable";
  m_TracePoints[TracePoint::WORKQUEUE] = mountPoint + "/tracing/events/workqueue/enable";
  m_TracePoints[TracePoint::MM_VMSCAN_DIRECT_RECLAIM_BEGIN] = mountPoint + "/tracing/events/vmscan/mm_vmscan_direct_reclaim_begin/enable";
  m_TracePoints[TracePoint::MM_VMSCAN_DIRECT_RECLAIM_END] = mountPoint + "/tracing/events/vmscan/mm_vmscan_direct_reclaim_end/enable";
  m_TracePoints[TracePoint::MM_VMSCAN_KSWAPD_WAKE] = mountPoint + "/tracing/events/vmscan/mm_vmscan_kswapd_wake/enable";
  m_TracePoints[TracePoint::MM_VMSCAN_KSWAPD_SLEEP] = mountPoint + "/tracing/events/vmscan/mm_vmscan_kswapd_sleep/enable";
  m_TracePoints[TracePoint::REGULATOR] = mountPoint + "/tracing/events/regulator/enable";
  m_TracePoints[TracePoint::BINDER_TRANSACTION] = mountPoint + "/tracing/events/binder/binder_transaction/enable";
  m_TracePoints[TracePoint::BINDER_TRANSACTION_RECEIVED] = mountPoint + "/tracing/events/binder/binder_transaction_received/enable";
  m_TracePoints[TracePoint::BINDER_LOCK] = mountPoint + "/tracing/events/binder/binder_lock/enable";
  m_TracePoints[TracePoint::BINDER_LOCKED] = mountPoint + "/tracing/events/binder/binder_locked/enable";
  m_TracePoints[TracePoint::BINDER_UNLOCK] = mountPoint + "/tracing/events/binder/binder_unlock/enable";
  m_TracePoints[TracePoint::FILEMAP] = mountPoint + "/tracing/events/filemap/enable";

  m_Options[Option::OVERWRITE] = mountPoint + "/tracing/options/overwrite";
  m_Options[Option::FUNCGRAPH_ABSTIME] = mountPoint + "/tracing/options/funcgraph-abstime";
  m_Options[Option::FUNCGRAPH_CPU] = mountPoint + "/tracing/options/funcgraph-cpu";
  m_Options[Option::FUNCGRAPH_PROC] = mountPoint + "/tracing/options/funcgraph-proc";
  m_Options[Option::PRINT_TGID] = mountPoint + "/tracing/options/print-tgid";

  m_ClockTypes[ClockType::GLOBAL] = "global";
  m_ClockTypes[ClockType::LOCAL] = "local";

  m_Tracers[Tracer::NOP] = "nop";
  m_Tracers[Tracer::FUNCTION_GRAPH] = "function_graph";
}

bool FTrace::tracePointAccessable(const FTrace::TracePoint & tracePoint) const
{
  return m_FileSystem->fileIsWritable(m_TracePoints.at(tracePoint).c_str());
}

bool FTrace::tracePointExists(const FTrace::TracePoint & tracePoint) const
{
  return m_FileSystem->fileExists(m_TracePoints.at(tracePoint).c_str());
}

bool FTrace::tryEnableTracePoint(const FTrace::TracePoint & tracePoint)
{
  return tryConfigTracePoint(tracePoint, true);
}

bool FTrace::tryDisableTracePoint(const FTrace::TracePoint & tracePoint)
{
  if (!tryConfigTracePoint(tracePoint, false)) {
    // fprintf(m_Wire.getErrorStream(), "Can't disable tracepoint: %s\n",
    //                                   m_TracePoints.at(tracePoint).c_str());
    return false;
  }
  return true;
}

bool FTrace::tryConfigTracePoint(const FTrace::TracePoint & tracePoint, bool enable)
{
  return tryConfigFileSwitch(m_TracePoints.at(tracePoint).c_str(), enable);
}

bool FTrace::tryEnableOption(const FTrace::Option & option)
{
  if (!tryConfigFileSwitch(m_Options.at(option).c_str(), true)) {
    fprintf(m_Wire.getErrorStream(), "can't enable property\n");
    return false;
  }
  return true;
}

bool FTrace::tryDisableOption(const FTrace::Option & option)
{
  return tryConfigFileSwitch(m_Options.at(option).c_str(), false);
}

bool FTrace::tryConfigFileSwitch(const char * filename, bool enable)
{
  if (!m_FileSystem->fileIsWritable(filename)) {
    fprintf(m_Wire.getErrorStream(), "file is not writable %s\n", filename);
    return false;
  }
  return m_FileSystem->writeStr(filename, enable? "1" : "0");
}

int FTrace::getTracePipeFd()
{
  return tryOpenFile(m_TracePipePath.c_str());
}

int FTrace::getTraceFd()
{
  return tryOpenFile(m_TracePath.c_str());
}

int FTrace::tryOpenFile(const char * filename)
{
  int traceFD = open(filename, O_RDWR);
  if (traceFD == -1) {
     fprintf(m_Wire.getErrorStream(), "error opening %s: %s (%d)\n", filename,
              strerror(errno), errno);
  }
  return traceFD;
}

bool FTrace::tryCleanTrace()
{
  return m_FileSystem->truncateFile(m_TracePath.c_str());
}

bool FTrace::trySetBufferSize(uint32_t bufferSize)
{
  if (bufferSize == 0) {
    fprintf(m_Wire.getErrorStream(), "error FTrace: bufferSize can't be set to 0\n");    
  }
  char str[32];
  snprintf(str, 32, "%u", bufferSize);
  if (!m_FileSystem->writeStr(m_TraceBufferSizePath.c_str(), str)) {
    fprintf(m_Wire.getErrorStream(), "can't set buffer size\n");
    return false;
  }
  return true;
}

bool FTrace::trySetClockType(const ClockType & clockType)
{
  return m_FileSystem->writeStr(m_TraceClockPath.c_str(), m_ClockTypes.at(clockType).c_str());
}

bool FTrace::tryStartTrace()
{
  return tryConfigFileSwitch(m_TracingOnPath.c_str(), true);
}

bool FTrace::tryStopTrace()
{
  return tryConfigFileSwitch(m_TracingOnPath.c_str(), false);
}

const set<string> FTrace::tryGetFunctionsFromFilter() const {
  char buffer[4097];    
  if (!m_FileSystem->readStr(m_TraceFilterPath.c_str(), buffer, 4097)) {
    return set<string>();
  }
  set<string> filters;
  m_ToolBox->parseToTokens(buffer, "\n", filters);
  // remove wildcards
  for(auto it = filters.begin(); it != filters.end(); )
  {
    if(it->find('*') != string::npos)
    {
      it = filters.erase(it);
    }
    else
    {
      ++it;
    }
  }
  return filters;
}

bool FTrace::tryWriteMarker(const char * marker) {
  return m_FileSystem->writeStr(m_TraceMarkerPath.c_str(), marker);
}


bool FTrace::hasTraceClockSetTo(const ClockType & clockType) const
{
    char buf[4097];    
    if (!m_FileSystem->readStr(m_TraceClockPath.c_str(), buf, 4097)) {
        return false;
    }

    char *start = strchr(buf, '[');
    if (start == NULL) {
        return false;
    }
    start++;

    char *end = strchr(start, ']');
    if (end == NULL) {
        return false;
    }
    *end = '\0';

    return strcmp(m_ClockTypes.at(clockType).c_str(), start) == 0;
}

bool FTrace::trySetTracer(const Tracer & tracer) {
  return m_FileSystem->writeStr(m_CurrentTracerPath.c_str(), m_Tracers.at(tracer).c_str());
}

bool FTrace::tryClearFunctionFilter() {
  return m_FileSystem->truncateFile(m_TraceFilterPath.c_str());
}

bool FTrace::tryAddFunctionToFilter(const string & function) {
  return m_FileSystem->appendStr(m_TraceFilterPath.c_str(), function.c_str());
}

bool FTrace::tracerChoiceAccessable() const {
  return m_FileSystem->fileIsWritable(m_CurrentTracerPath.c_str());
}

bool FTrace::functionFilterAccessible() const {
  return m_FileSystem->fileIsWritable(m_TraceFilterPath.c_str());
}