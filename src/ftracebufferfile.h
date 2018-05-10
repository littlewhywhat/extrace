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

#ifndef LTTLWHWHT_FTRACEBUFFERFILE_H
#define LTTLWHWHT_FTRACEBUFFERFILE_H

#include "wired.h"
#include "ftrace.h"
#include "filedatamaker.h"
#include "signal.h"

//! I am a buffer file of ftrace. I can support many operations with myself

//! I can stream, send my data and write in myself. I can switch to circular mode,
//! set my size or clear myself.
class FTraceBufferFile : public Wired {
  public:
    //! Creates me from FTrace with my filedata maker
    FTraceBufferFile(const Wire & wire, FTrace * ftrace,
                    FileDataMaker * fileDataMaker):
                    Wired(wire), m_FTrace(ftrace), m_FileDataMaker(fileDataMaker) {}
    //! Streams me to Wire output until Signal fires 
    bool tryStreamUntil(const Signal & signal);
    //! Sends me to file descriptor
    bool trySendTo(int fd);
    //! Writes into me sync marker
    bool tryWriteSyncMarker();
    //! Transforms me to circular
    bool trySetCircularMode();
    //! Sets my size
    bool trySetSize(uint32_t size_t);
    //! Clears me
    bool tryClear();
  private:
    //! my ftrace
    FTrace * m_FTrace = NULL;
    //! my data maker
    FileDataMaker * m_FileDataMaker = NULL;
};

#endif // LTTLWHWHT_FTRACEBUFFERFILE_H