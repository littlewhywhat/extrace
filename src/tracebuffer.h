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

#ifndef LTTLWHWHT_TRACEBUFFER_H
#define LTTLWHWHT_TRACEBUFFER_H

#include "wired.h"
#include "signal.h"

//! I am a buffer of trace. I can support many operations with myself

//! I can stream, send my data and write in myself. I can switch to circular mode,
//! set my size or clear myself.
class TraceBuffer : public Wired {
  public:
    //! Constructs me
    TraceBuffer(const Wire & wire): Wired(wire) {}
    //! Destroys me
    virtual ~TraceBuffer() {}
    //! Streams me to Wire output until Signal fires 
    virtual bool tryStreamUntil(const Signal & signal) = 0;
    //! Sends me to file descriptor
    virtual bool trySendTo(int fd) = 0;
    //! Writes into me sync marker
    virtual bool tryWriteSyncMarker() = 0;
    //! Writes into me a string
    virtual bool tryWriteString(const char * string) = 0;
    //! Transforms me to circular
    virtual bool trySetCircularMode() = 0;
    //! Sets my size
    virtual bool trySetSize(uint32_t size_t) = 0;
    //! Clears me
    virtual bool tryClear() = 0;
};

#endif // LTTLWHWHT_TRACEBUFFER_H