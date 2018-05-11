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

#include "tracebuffer.h"
#include "ftrace.h"
#include "filedatamaker.h"
#include "signal.h"

#include <memory>

//! I am a buffer file of ftrace. I can support many operations with myself

//! I can stream, send my data and write in myself. I can switch to circular mode,
//! set my size or clear myself.
class FTraceBufferFile : public TraceBuffer {
  public:
    //! Creates me from FTrace with my filedata maker
    FTraceBufferFile(const Wire & wire, const shared_ptr<FTrace> & ftrace,
                     FileDataMaker * fileDataMaker):
                     TraceBuffer(wire), m_FTrace(ftrace), m_FileDataMaker(fileDataMaker) {}
    //! Destroys me and my FileDataMaker
    ~FTraceBufferFile();
    //! Streams me to Wire output until Signal fires 
    bool tryStreamUntil(const Signal & signal) override;
    //! Sends me to file descriptor
    bool trySendTo(int fd) override;
    //! Writes into me sync marker
    bool tryWriteSyncMarker() override;
    bool tryWriteString(const char * string) override;
    //! Transforms me to circular
    bool trySetCircularMode() override;
    //! Sets my size
    bool trySetSize(uint32_t size_t) override;
    //! Clears me
    bool tryClear() override;
  private:
    //! my ftrace
    shared_ptr<FTrace> m_FTrace;
    //! my data maker
    FileDataMaker * m_FileDataMaker = NULL;
};

#endif // LTTLWHWHT_FTRACEBUFFERFILE_H