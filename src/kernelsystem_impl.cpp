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
{}

bool KernelSystemImpl::trySendTraceTo(int outFD) {
  int traceFD = getTraceFd();
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
  int traceFD = getTraceFd();
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
  int traceStream = getTracePipeFd();
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

bool KernelSystemImpl::setKernelOptionEnable(const char* filename, bool enable)
{
    return m_FileSystem->writeStr(filename, enable ? "1" : "0");
}

bool KernelSystemImpl::isPossibleSetKernelOption(const char* filename)
{
    return filename != NULL && m_FileSystem->fileIsWritable(filename);
}

bool KernelSystemImpl::isCategorySupported(const TracingCategory& category) const
{
    if (_isCategorySupported(category)) {
        return true;
    } else {
        if (isCategorySupportedForRoot(category)) {
            fprintf(m_Wire.getErrorStream(), "error: category \"%s\" requires root "
                    "privileges.\n", category.name);
        } else {
            fprintf(m_Wire.getErrorStream(), "error: category \"%s\" is not supported "
                    "on this device.\n", category.name);
        }
        return false;
    }
}

bool KernelSystemImpl::_isCategorySupported(const TracingCategory& category) const
{
    bool ok = true;
    for (const auto & file : category.files) {
        const char* path = file.path;
        bool req = file.required == EnableFile::REQ;
        if (path != NULL) {
            if (req) {
                if (!m_FileSystem->fileIsWritable(path)) {
                    fprintf(m_Wire.getErrorStream(), "File %s is not writable\n", path);
                    return false;
                } else {
                    ok = true;
                }
            } else {
                ok |= m_FileSystem->fileIsWritable(path);
            }
        }
    }
    return ok;
}

bool KernelSystemImpl::isCategorySupportedForRoot(const TracingCategory& category) const
{
    bool ok = category.tags != 0;
    for (const auto & file : category.files) {
        const char* path = file.path;
        bool req = file.required == EnableFile::REQ;
        if (path != NULL) {
            if (req) {
                if (!m_FileSystem->fileExists(path)) {
                    return false;
                } else {
                    ok = true;
                }
            } else {
                ok |= m_FileSystem->fileExists(path);
            }
        }
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
  ok &= writeMarker(buffer);
  int64_t realtime_in_ms = m_SystemTime->get_realtime();
  snprintf(buffer, 128, "trace_event_clock_sync: realtime_ts=%" PRId64 "\n", realtime_in_ms);
  ok &= writeMarker(buffer);
  return ok;
}

bool KernelSystemImpl::writeMarker(const char * buffer)
{
  return m_FileSystem->writeStr(k_traceMarkerPath, buffer);
}

bool KernelSystemImpl::setTraceOverwriteEnable(bool enable)
{
  return setKernelOptionEnable(k_tracingOverwriteEnablePath, enable);
}

bool KernelSystemImpl::setTracingEnabled(bool enable)
{
    return setKernelOptionEnable(k_tracingOnPath, enable);
}

bool KernelSystemImpl::clearTrace()
{
    return m_FileSystem->truncateFile(k_tracePath);
}

int KernelSystemImpl::getTracePipeFd()
{
    int traceFD = open(k_traceStreamPath, O_RDWR);
    if (traceFD == -1) {
       fprintf(m_Wire.getErrorStream(), "error opening %s: %s (%d)\n", k_traceStreamPath,
                strerror(errno), errno);
    }
    return traceFD;
}

int KernelSystemImpl::getTraceFd()
{
    int traceFD = open(k_tracePath, O_RDWR);
    if (traceFD == -1) {
       fprintf(m_Wire.getErrorStream(), "error opening %s: %s (%d)\n", k_tracePath,
                strerror(errno), errno);
    }
    return traceFD;
}

bool KernelSystemImpl::setTraceBufferSizeKB(int size)
{
    char str[32] = "1";
    if (size < 1) {
        size = 1;
    }
    snprintf(str, 32, "%d", size);
    return m_FileSystem->writeStr(k_traceBufferSizePath, str);
}

bool KernelSystemImpl::setGlobalClockEnable(bool enable)
{
    const char *clock = enable ? "global" : "local";

    if (isTraceClock(clock)) {
        return true;
    }

    return m_FileSystem->writeStr(k_traceClockPath, clock);
}

bool KernelSystemImpl::setPrintTgidEnableIfPresent(bool enable)
{
    if (m_FileSystem->fileExists(k_printTgidPath)) {
        return setKernelOptionEnable(k_printTgidPath, enable);
    }
    return true;
}

bool KernelSystemImpl::setKernelTraceFuncs(const vector<string> & funcs)
{
    bool ok = true;

    if (funcs.empty()) {
        // Disable kernel function tracing.
        if (m_FileSystem->fileIsWritable(k_currentTracerPath)) {
            ok &= m_FileSystem->writeStr(k_currentTracerPath, "nop");
        }
        if (m_FileSystem->fileIsWritable(k_ftraceFilterPath)) {
            ok &= m_FileSystem->truncateFile(k_ftraceFilterPath);
        }
    } else {
        // Enable kernel function tracing.
        ok &= m_FileSystem->writeStr(k_currentTracerPath, "function_graph");
        ok &= setKernelOptionEnable(k_funcgraphAbsTimePath, true);
        ok &= setKernelOptionEnable(k_funcgraphCpuPath, true);
        ok &= setKernelOptionEnable(k_funcgraphProcPath, true);
        // ok &= setKernelOptionEnable(k_funcgraphFlatPath, true);

        // Set the requested filter functions.
        ok &= m_FileSystem->truncateFile(k_ftraceFilterPath);
        for (const auto & func: funcs) {
            ok &= m_FileSystem->appendStr(k_ftraceFilterPath, func.c_str());
        }

        // Verify that the set functions are being traced.
        if (ok) {
            ok &= verifyKernelTraceFuncs(funcs);
        }
    }

    return ok;
}

bool KernelSystemImpl::isTraceClock(const char *mode)
{
    char buf[4097];    
    if (!m_FileSystem->readStr(k_traceClockPath, buf, 4097)) {
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

    return strcmp(mode, start) == 0;
}

bool KernelSystemImpl::verifyKernelTraceFuncs(const vector<string> & funcs) const
{
    char buf[4097];    
    if (!m_FileSystem->readStr(k_ftraceFilterPath, buf, 4097)) {
        return false;
    }

    std::set<std::string> funcs_traced;
    m_ToolBox->parseToTokens(buf, "\n", funcs_traced);

    bool ok = true;
    for (const auto & func : funcs) {
        // except wildcards
        if (strchr(func.c_str(), '*') == NULL 
            && funcs_traced.find(func) == funcs_traced.end()) {
            fprintf(m_Wire.getErrorStream(), "error: \"%s\" is not a valid kernel function to trace.\n",
                    func.c_str());
            ok = false;
        }
    }
    return ok;
}

void KernelSystemImpl::add_kernel_category(const char * id, const char * name, const std::vector<EnableFile> &files)
{
  m_Categories[id] = {id, name, 0, files, false };
  m_CategoriesList.push_back({id, name, 0, files, false });
}

// Disable all /sys/ enable files.
bool KernelSystemImpl::disableKernelTraceEvents() {
    bool ok = true;
    for (const auto & category : m_CategoriesList) {
        for (const auto & file : category.files) {
            const char* path = file.path;
            if (isPossibleSetKernelOption(path)) {
                ok &= setKernelOptionEnable(path, false);
            }
        }
    }
    return ok;
}

bool KernelSystemImpl::setKernelTraceCategories(const std::vector<string> & ids) {
    bool ok = true;
    for (const auto & id : ids) {
        if (m_Categories.find(id) == m_Categories.end()) {
            fprintf(m_Wire.getErrorStream(), "wrong kernel category id '%s'\n", id.c_str());
            return false;
        } else {
            const auto & category = m_Categories[id];
            for (const auto & file : category.files) {
                const char* path = file.path;
                bool required = file.required == EnableFile::REQ;
                if (path != NULL) {
                    if (isPossibleSetKernelOption(path)) {
                        ok &= setKernelOptionEnable(path, true);
                    } else if (required) {
                        fprintf(m_Wire.getErrorStream(), "error writing file %s\n", path);
                        ok = false;
                    }
                }
            }
        }
    }
    return ok;
}

const vector<TracingCategory> & KernelSystemImpl::getCategories() const {
    return m_CategoriesList;
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
