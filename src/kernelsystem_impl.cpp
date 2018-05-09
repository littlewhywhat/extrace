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

#include <errno.h>  // errno
#include <string.h> // strerror
#include <fcntl.h>  // creat, ope, O_WRONLY, O_CREAT
#include <stdlib.h> // free
#include <unistd.h> // read, close
#include <stdio.h>  // FILE
#include <set>
#include <sys/sendfile.h>
#include <zlib.h>

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
  bool ok = try_sendfile(traceFD, outFD);
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
  bool ok = compress_trace_to(traceFD, outFD);
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error KernelSystemImpl::trySendTraceCompressedTo\n");
  }
  close(traceFD);
  ok &= clearTrace();
  return ok;
}

bool KernelSystemImpl::tryStreamTrace(const Signal & signal) {
  bool ok = true;
  int traceStream = m_FTrace->getTracePipeFd();
  if (traceStream == -1) {
      fprintf(m_Wire.getErrorStream(), "error KernelSystemImpl::tryStreamTraceTo\n");
      return false;
  }
  FILE * outputStream = m_Wire.getOutputStream();
  while (!signal.isFired()) {
      if (!try_send(traceStream, fileno(outputStream))) {
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

bool KernelSystemImpl::try_sendfile(int fd_from, int fd_to)
{
    ssize_t sent = 0;
    while ((sent = sendfile(fd_to, fd_from, NULL, 64*1024*1024)) > 0);
    if (sent == -1) {
        fprintf(m_Wire.getErrorStream(), "error sendfile: %s (%d)\n", strerror(errno),
                errno);
        return false;
    }
    return true;
}

bool KernelSystemImpl::try_send(int fd_from, int fd_to) {
    char trace_data[4096];
    ssize_t bytes_read = read(fd_from, trace_data, 4096);
    if (bytes_read > 0) {
        write(fd_to, trace_data, bytes_read);
        return true;
    }
    return false;
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

bool KernelSystemImpl::compress_trace_to(int traceFD, int outFd) {
    z_stream zs;
    uint8_t *in, *out;
    int result, flush;

    memset(&zs, 0, sizeof(zs));
    result = deflateInit(&zs, Z_DEFAULT_COMPRESSION);
    if (result != Z_OK) {
        fprintf(m_Wire.getErrorStream(), "error initializing zlib: %d\n", result);
        close(traceFD);
        return false;
    }

    bool ok = true;
    const size_t bufSize = 64*1024;
    in = (uint8_t*)malloc(bufSize);
    out = (uint8_t*)malloc(bufSize);
    flush = Z_NO_FLUSH;

    zs.next_out = out;
    zs.avail_out = bufSize;

    do {

        if (zs.avail_in == 0) {
            // More input is needed.
            result = read(traceFD, in, bufSize);
            if (result < 0) {
                fprintf(m_Wire.getErrorStream(), "error reading trace: %s (%d)\n",
                        strerror(errno), errno);
                result = Z_STREAM_END;
                ok = false;
                break;
            } else if (result == 0) {
                flush = Z_FINISH;
            } else {
                zs.next_in = in;
                zs.avail_in = result;
            }
        }

        if (zs.avail_out == 0) {
            // Need to write the output.
            result = write(outFd, out, bufSize);
            if ((size_t)result < bufSize) {
                fprintf(m_Wire.getErrorStream(), "error writing deflated trace: %s (%d)\n",
                        strerror(errno), errno);
                result = Z_STREAM_END; // skip deflate error message
                zs.avail_out = bufSize; // skip the final write
                ok = false;
                break;
            }
            zs.next_out = out;
            zs.avail_out = bufSize;
        }

    } while ((result = deflate(&zs, flush)) == Z_OK);

    if (result != Z_STREAM_END) {
        fprintf(m_Wire.getErrorStream(), "error deflating trace: %s\n", zs.msg);
        ok = false;
    }

    if (zs.avail_out < bufSize) {
        size_t bytes = bufSize - zs.avail_out;
        result = write(outFd, out, bytes);
        if ((size_t)result < bytes) {
            fprintf(m_Wire.getErrorStream(), "error writing deflated trace: %s (%d)\n",
                    strerror(errno), errno);
            ok = false;
        }
    }

    result = deflateEnd(&zs);
    if (result != Z_OK) {
        fprintf(m_Wire.getErrorStream(), "error cleaning up zlib: %d\n", result);
        ok = false;
    }

    free(in);
    free(out);
    return ok;
}
