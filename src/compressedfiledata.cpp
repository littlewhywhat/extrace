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

#include "compressedfiledata.h"

#include <string.h> // strerror
#include <errno.h>  // errno
#include <stdlib.h> // free
#include <unistd.h> // read, close
#include <zlib.h>

bool CompressedFileData::trySendTo(int fd) const {
  z_stream zs;
  uint8_t *in, *out;
  int result, flush;

  memset(&zs, 0, sizeof(zs));
  result = deflateInit(&zs, Z_DEFAULT_COMPRESSION);
  if (result != Z_OK) {
      fprintf(m_Wire.getErrorStream(), "error initializing zlib: %d\n", result);
      close(m_Fd);
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
          result = read(m_Fd, in, bufSize);
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
          result = write(fd, out, bufSize);
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
      result = write(fd, out, bytes);
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
