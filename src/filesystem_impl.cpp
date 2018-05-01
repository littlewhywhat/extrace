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

#include "filesystem_impl.h"

#include <errno.h>  // errno
#include <string.h> // strerror
#include <unistd.h> // access
#include <fcntl.h>  // creat, open

bool FileSystemImpl::fileExists(const char* filename)
{
    return access(filename, F_OK) != -1;
}

bool FileSystemImpl::fileIsWritable(const char* filename)
{
    return access(filename, W_OK) != -1;
}

bool FileSystemImpl::truncateFile(const char* path)
{
    // This uses creat rather than truncate because some of the debug kernel
    // device nodes (e.g. k_ftraceFilterPath) currently aren't changed by
    // calls to truncate, but they are cleared by calls to creat.
    int traceFD = creat(path, 0);
    if (traceFD == -1) {
        fprintf(m_Wire.getErrorStream(), "error truncating %s: %s (%d)\n", path,
            strerror(errno), errno);
        return false;
    }

    close(traceFD);

    return true;
}

bool FileSystemImpl::writeStr(const char* filename, const char* str)
{
    return _writeStr(filename, str, O_WRONLY);
}

bool FileSystemImpl::appendStr(const char* filename, const char* str)
{
    return _writeStr(filename, str, O_APPEND|O_WRONLY);
}

bool FileSystemImpl::_writeStr(const char* filename, const char* str, int flags)
{
    int fd = open(filename, flags);
    if (fd == -1) {
        fprintf(m_Wire.getErrorStream(), "error opening %s: %s (%d)\n", filename,
                strerror(errno), errno);
        return false;
    }

    bool ok = true;
    ssize_t len = strlen(str);
    if (write(fd, str, len) != len) {
        fprintf(m_Wire.getErrorStream(), "error writing to %s: %s (%d)\n", filename,
                strerror(errno), errno);
        ok = false;
    }

    close(fd);

    return ok;
}

bool FileSystemImpl::readStr(const char* filename, char* str, int max_size) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(m_Wire.getErrorStream(), "error opening %s: %s (%d)\n", filename,
            strerror(errno), errno);
        return false;
    }

    ssize_t n = read(fd, str, max_size - 1);
    close(fd);
    if (n == -1) {
        fprintf(m_Wire.getErrorStream(), "error reading %s: %s (%d)\n", filename,
            strerror(errno), errno);
        return false;
    }
    str[n] = '\0';
    return true;
}