#include "ftrace.h"

#include <errno.h>  // errno
#include <fcntl.h>  // creat, ope, O_WRONLY, O_CREAT

FTrace::~FTrace()
{
  delete m_ToolBox;
}

FTrace::FTrace(const Wire & wire, FileSystem * fileSystem, ToolBox * toolBox):
                      Wired(wire), m_FileSystem(fileSystem), m_ToolBox(toolBox)
{
  m_TracePoints[TracePoint::SCHED_SWITCH] = "/sys/kernel/debug/tracing/events/sched/sched_switch/enable";
  m_TracePoints[TracePoint::SCHED_WAKEUP] = "/sys/kernel/debug/tracing/events/sched/sched_wakeup/enable";
  m_TracePoints[TracePoint::SCHED_BLOCKED_REASON] = "/sys/kernel/debug/tracing/events/sched/sched_blocked_reason/enable";
  m_TracePoints[TracePoint::SCHED_CPU_HOTPLUG] = "/sys/kernel/debug/tracing/events/sched/sched_cpu_hotplug/enable";
  m_TracePoints[TracePoint::IRQ] = "/sys/kernel/debug/tracing/events/irq/enable";
  m_TracePoints[TracePoint::IPI] = "/sys/kernel/debug/tracing/events/ipi/enable";
  m_TracePoints[TracePoint::CPU_FREQUENCY] = "/sys/kernel/debug/tracing/events/power/cpu_frequency/enable";
  m_TracePoints[TracePoint::CLOCK_SET_RATE] = "/sys/kernel/debug/tracing/events/power/clock_set_rate/enable";
  m_TracePoints[TracePoint::CPU_FREQUENCY_LIMITS] = "/sys/kernel/debug/tracing/events/power/cpu_frequency_limits/enable";
  m_TracePoints[TracePoint::MEMORY_BUS] = "/sys/kernel/debug/tracing/events/memory_bus/enable";
  m_TracePoints[TracePoint::CPU_IDLE] = "/sys/kernel/debug/tracing/events/power/cpu_idle/enable";
  m_TracePoints[TracePoint::F2FS_SYNC_FILE_ENTER] = "/sys/kernel/debug/tracing/events/f2fs/f2fs_sync_file_enter/enable";
  m_TracePoints[TracePoint::F2FS_SYNC_FILE_EXIT] = "/sys/kernel/debug/tracing/events/f2fs/f2fs_sync_file_exit/enable";
  m_TracePoints[TracePoint::F2FS_WRITE_BEGIN] = "/sys/kernel/debug/tracing/events/f2fs/f2fs_write_begin/enable";
  m_TracePoints[TracePoint::F2FS_WRITE_END] = "/sys/kernel/debug/tracing/events/f2fs/f2fs_write_end/enable";
  m_TracePoints[TracePoint::EXT4_DA_WRITE_BEGIN] = "/sys/kernel/debug/tracing/events/ext4/ext4_da_write_begin/enable";
  m_TracePoints[TracePoint::EXT4_DA_WRITE_END] = "/sys/kernel/debug/tracing/events/ext4/ext4_da_write_end/enable";
  m_TracePoints[TracePoint::EXT4_SYNC_FILE_ENTER] = "/sys/kernel/debug/tracing/events/ext4/ext4_sync_file_enter/enable";
  m_TracePoints[TracePoint::EXT4_SYNC_FILE_EXIT] = "/sys/kernel/debug/tracing/events/ext4/ext4_sync_file_exit/enable";
  m_TracePoints[TracePoint::BLOCK_RQ_ISSUE] = "/sys/kernel/debug/tracing/events/block/block_rq_issue/enable";
  m_TracePoints[TracePoint::BLOCK_RQ_COMPLETE] = "/sys/kernel/debug/tracing/events/block/block_rq_complete/enable";
  m_TracePoints[TracePoint::MMC] = "/sys/kernel/debug/tracing/events/mmc/enable";
  m_TracePoints[TracePoint::CPUFREQ_INTERACTIVE] = "/sys/kernel/debug/tracing/events/cpufreq_interactive/enable";
  m_TracePoints[TracePoint::SYNC] = "/sys/kernel/debug/tracing/events/sync/enable";
  m_TracePoints[TracePoint::WORKQUEUE] = "/sys/kernel/debug/tracing/events/workqueue/enable";
  m_TracePoints[TracePoint::MM_VMSCAN_DIRECT_RECLAIM_BEGIN] = "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_begin/enable";
  m_TracePoints[TracePoint::MM_VMSCAN_DIRECT_RECLAIM_END] = "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_end/enable";
  m_TracePoints[TracePoint::MM_VMSCAN_KSWAPD_WAKE] = "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_wake/enable";
  m_TracePoints[TracePoint::MM_VMSCAN_KSWAPD_SLEEP] = "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_sleep/enable";
  m_TracePoints[TracePoint::REGULATOR] = "/sys/kernel/debug/tracing/events/regulator/enable";
  m_TracePoints[TracePoint::BINDER_TRANSACTION] = "/sys/kernel/debug/tracing/events/binder/binder_transaction/enable";
  m_TracePoints[TracePoint::BINDER_TRANSACTION_RECEIVED] = "/sys/kernel/debug/tracing/events/binder/binder_transaction_received/enable";
  m_TracePoints[TracePoint::BINDER_LOCK] = "/sys/kernel/debug/tracing/events/binder/binder_lock/enable";
  m_TracePoints[TracePoint::BINDER_LOCKED] = "/sys/kernel/debug/tracing/events/binder/binder_locked/enable";
  m_TracePoints[TracePoint::BINDER_UNLOCK] = "/sys/kernel/debug/tracing/events/binder/binder_unlock/enable";
  m_TracePoints[TracePoint::FILEMAP] = "/sys/kernel/debug/tracing/events/filemap/enable";

  m_Options[Option::OVERWRITE] = "/sys/kernel/debug/tracing/options/overwrite";
  m_Options[Option::FUNCGRAPH_ABSTIME] = "/sys/kernel/debug/tracing/options/funcgraph-abstime";
  m_Options[Option::FUNCGRAPH_CPU] = "/sys/kernel/debug/tracing/options/funcgraph-cpu";
  m_Options[Option::FUNCGRAPH_PROC] = "/sys/kernel/debug/tracing/options/funcgraph-proc";
  m_Options[Option::PRINT_TGID] = "/sys/kernel/debug/tracing/options/print-tgid";

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
  return tryConfigTracePoint(tracePoint, false);
}

bool FTrace::tryConfigTracePoint(const FTrace::TracePoint & tracePoint, bool enable)
{
  return tryConfigFileSwitch(m_TracePoints.at(tracePoint).c_str(), enable);
}

bool FTrace::tryEnableOption(const FTrace::Option & option)
{
  return tryConfigFileSwitch(m_Options.at(option).c_str(), true);
}

bool FTrace::tryDisableOption(const FTrace::Option & option)
{
  return tryConfigFileSwitch(m_Options.at(option).c_str(), false);
}

bool FTrace::tryConfigFileSwitch(const char * filename, bool enable)
{
  return m_FileSystem->fileIsWritable(filename)
         && m_FileSystem->writeStr(filename, enable? "1" : "0");
}

int FTrace::getTracePipeFd()
{
  return tryOpenFile(m_TracePipePath);
}

int FTrace::getTraceFd()
{
  return tryOpenFile(m_TracePath);
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
  return m_FileSystem->truncateFile(m_TracePath);
}

bool FTrace::trySetBufferSize(uint32_t bufferSize)
{
  if (bufferSize == 0) {
    fprintf(m_Wire.getErrorStream(), "error FTrace: bufferSize can't be set to 0\n");    
  }
  char str[32];
  snprintf(str, 32, "%u", bufferSize);
  return m_FileSystem->writeStr(m_TraceBufferSizePath, str);
}

bool FTrace::trySetClockType(const ClockType & clockType)
{
  return m_FileSystem->writeStr(m_TraceClockPath, m_ClockTypes.at(clockType).c_str());
}

bool FTrace::tryStartTrace()
{
  return tryConfigFileSwitch(m_TracingOnPath, true);
}

bool FTrace::tryStopTrace()
{
  return tryConfigFileSwitch(m_TracingOnPath, false);
}

const set<string> FTrace::tryGetFunctionsFromFilter() const {
  char buffer[4097];    
  if (!m_FileSystem->readStr(m_TraceFilterPath, buffer, 4097)) {
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
  return m_FileSystem->writeStr(m_TraceMarkerPath, marker);
}


bool FTrace::hasTraceClockSetTo(const ClockType & clockType) const
{
    char buf[4097];    
    if (!m_FileSystem->readStr(m_TraceClockPath, buf, 4097)) {
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
  return m_FileSystem->writeStr(m_CurrentTracerPath, m_Tracers.at(tracer).c_str());
}

bool FTrace::tryClearFunctionFilter() {
  return m_FileSystem->truncateFile(m_TraceFilterPath);
}

bool FTrace::tryAddFunctionToFilter(const string & function) {
  return m_FileSystem->appendStr(m_TraceFilterPath, function.c_str());
}

bool FTrace::tracerChoiceAccessable() const {
  return m_FileSystem->fileIsWritable(m_CurrentTracerPath);
}

bool FTrace::functionFilterAccessible() const {
  return m_FileSystem->fileIsWritable(m_TraceFilterPath);
}