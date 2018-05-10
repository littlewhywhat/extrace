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

#include "kernelsystem_impl.h"

#include <unistd.h> // read, close

#include "compressedfiledata.h"
#include "simplefiledata.h"
#include "filedataslice.h"

KernelSystemImpl::~KernelSystemImpl()
{
  delete m_ToolBox;
  delete m_SystemTime;
  delete m_FTrace;
}

bool KernelSystemImpl::trySendTraceTo(int outFD) {
  int traceFD = m_FTrace->getTraceFd();
  if (traceFD == -1) {
    fprintf(m_Wire.getErrorStream(), "error KernelSystemImpl::trySendTraceTo\n");
    return false;
  }
  dprintf(outFD, "TRACE:\n");
  bool ok = SimpleFileData(m_Wire, traceFD).trySendTo(outFD);
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error KernelSystemImpl::trySendTraceTo\n");
  }
  close(traceFD);
  ok &= clearTrace();
  return ok;
}

bool KernelSystemImpl::trySendTraceCompressedTo(int outFD) {
  int traceFD = m_FTrace->getTraceFd();
  if (traceFD == -1) {
    fprintf(m_Wire.getErrorStream(), "error KernelSystemImpl::trySendTraceCompressedTo\n");
    return false;
  }
  dprintf(outFD, "TRACE:\n");
  bool ok = CompressedFileData(m_Wire, traceFD).trySendTo(outFD);
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error KernelSystemImpl::trySendTraceCompressedTo\n");
  }
  close(traceFD);
  ok &= clearTrace();
  return ok;
}

bool KernelSystemImpl::tryStreamTrace(const Signal & signal) {
  bool ok = true;
  int tracePipeFD = m_FTrace->getTracePipeFd();
  if (tracePipeFD == -1) {
      fprintf(m_Wire.getErrorStream(), "error KernelSystemImpl::tryStreamTraceTo\n");
      return false;
  }
  FILE * outputStream = m_Wire.getOutputStream();
  FileDataSlice fileDataSlice(m_Wire, tracePipeFD);
  while (!signal.isFired()) {
      if (!fileDataSlice.trySendTo(fileno(outputStream))) {
          if (!signal.isFired()) {
            fprintf(m_Wire.getErrorStream(), "error KernelSystemImpl::tryStreamTraceTo - stream aborted\n");
            ok = false;
          }
          break;
      }
      fflush(outputStream);
  }
  return ok;
}

bool KernelSystemImpl::writeClockSyncMarker()
{
  printf("writeClockSyncMarker\n");
  char buffer[128];
  float now_in_seconds = m_SystemTime->get_monotonic();
  snprintf(buffer, 128, "trace_event_clock_sync: parent_ts=%f\n", now_in_seconds);
  bool ok = true;
  ok &= m_FTrace->tryWriteMarker(buffer);
  int64_t realtime_in_ms = m_SystemTime->get_realtime();
  snprintf(buffer, 128, "trace_event_clock_sync: realtime_ts=%" PRId64 "\n", realtime_in_ms);
  ok &= m_FTrace->tryWriteMarker(buffer);
  return ok;
}

bool KernelSystemImpl::setTraceOverwriteEnable(bool enable)
{
  return enable? m_FTrace->tryEnableOption(FTrace::Option::OVERWRITE):
                 m_FTrace->tryDisableOption(FTrace::Option::OVERWRITE);
}

bool KernelSystemImpl::setTracingEnabled(bool enable)
{
  return enable? m_FTrace->tryStartTrace() : m_FTrace->tryStopTrace();
}

bool KernelSystemImpl::clearTrace()
{
    return m_FTrace->tryCleanTrace();
}

bool KernelSystemImpl::setTraceBufferSizeKB(int size)
{
    return m_FTrace->trySetBufferSize(size);
}

bool KernelSystemImpl::setGlobalClockEnable(bool enable)
{
    const FTrace::ClockType clockType = enable ? FTrace::ClockType::GLOBAL : FTrace::ClockType::LOCAL;

    if (m_FTrace->hasTraceClockSetTo(clockType)) {
      return true;
    }
    return m_FTrace->trySetClockType(clockType);
}

bool KernelSystemImpl::setPrintTgidEnableIfPresent(bool enable)
{
  return enable? m_FTrace->tryEnableOption(FTrace::Option::PRINT_TGID):
                 m_FTrace->tryDisableOption(FTrace::Option::PRINT_TGID);
}
