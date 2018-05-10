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

#include "ftracebufferfile.h"

#include "filedataslice.h"
#include "systemtime_impl.h"
#include <unistd.h> // read, close
#include <memory>   // unique_ptr

using namespace std;

FTraceBufferFile::~FTraceBufferFile() {
  delete m_FileDataMaker;
}

bool FTraceBufferFile::tryStreamUntil(const Signal & signal) {
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

bool FTraceBufferFile::trySendTo(int fd) {
  int traceFD = m_FTrace->getTraceFd();
  if (traceFD == -1) {
    fprintf(m_Wire.getErrorStream(), "error KernelSystemImpl::trySendTraceTo\n");
    return false;
  }
  dprintf(fd, "TRACE:\n");
  const auto fileData = unique_ptr<FileData>(m_FileDataMaker->tryMake(m_Wire, traceFD));
  bool ok = fileData->trySendTo(fd);
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error KernelSystemImpl::trySendTraceTo\n");
  }
  close(traceFD);
  ok &= tryClear();
  return ok;
}

bool FTraceBufferFile::tryWriteSyncMarker() {
  char buffer[128];
  SystemTimeImpl systemTime;
  float now_in_seconds = systemTime.get_monotonic();
  snprintf(buffer, 128, "trace_event_clock_sync: parent_ts=%f\n", now_in_seconds);
  bool ok = true;
  ok &= m_FTrace->tryWriteMarker(buffer);
  int64_t realtime_in_ms = systemTime.get_realtime();
  snprintf(buffer, 128, "trace_event_clock_sync: realtime_ts=%" PRId64 "\n", realtime_in_ms);
  ok &= m_FTrace->tryWriteMarker(buffer);
  return ok;
}

bool FTraceBufferFile::trySetCircularMode() {
  return m_FTrace->tryEnableOption(FTrace::Option::OVERWRITE);
}

bool FTraceBufferFile::trySetSize(uint32_t size) {
  return m_FTrace->trySetBufferSize(size);
}

bool FTraceBufferFile::tryClear() {
  return m_FTrace->tryCleanTrace();
}
