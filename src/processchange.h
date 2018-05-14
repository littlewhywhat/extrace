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

#ifndef LTTLWHWHT_PROCESS_CHANGE_H
#define LTTLWHWHT_PROCESS_CHANGE_H

#include "process.h"

#include <string>
#include <cstdint>

//! I am a change of Process. You can apply me to Process.
class ProcessChange {
  public:
    ProcessChange(int pid, uint64_t timestamp):
                  myPID(pid), myTimeStamp(timestamp) {}
    virtual ~ProcessChange() {}
    int getPID()        const { return myPID; }
    uint64_t getTimeStamp() const { return myTimeStamp; }
    const std::string & getCause() const { return myCause; }

    ProcessChange * setPID(int pid)        { myPID = pid;       return this; }
    ProcessChange * setTimeStamp(uint64_t time) { myTimeStamp = time; return this; }
    ProcessChange * setCause(const std::string & cause) { myCause = cause; return this; }

    //! Applies me to process
    void applyTo(Process & process) const;
  protected:
    virtual void applyFurtherTo(Process & process) const = 0;
  private:
    int myPID;
    uint64_t myTimeStamp;
    std::string myCause;
};

#endif // LTTLWHWHT_PROCESS_CHANGE_H

#ifndef LTTLWHWHT_MEMORY_CHANGE_H
#define LTTLWHWHT_MEMORY_CHANGE_H

class MemoryChange : public ProcessChange {
  public:
    MemoryChange(int pid, uint64_t timestamp):
                 ProcessChange(pid, timestamp) {}
    ~MemoryChange() {}

    long getVSS() const { return myVSS; }
    long getRSS() const { return myRSS; }
    long getPSS() const { return myPSS; }
    long getUSS() const { return myUSS; }

    MemoryChange * setVSS(long vss) { myVSS = vss; return this; }
    MemoryChange * setRSS(long rss) { myRSS = rss; return this; }
    MemoryChange * setPSS(long pss) { myPSS = pss; return this; }
    MemoryChange * setUSS(long uss) { myUSS = uss; return this; }
  protected:
    //! Applies me to process
    void applyFurtherTo(Process & process) const override;
  private:
    long myVSS = -1;
    long myRSS = -1;
    long myPSS = -1;
    long myUSS = -1;
};

#endif // LTTLWHWHT_MEMORY_CHANGE_H

#ifndef LTTLWHWHT_STATE_CHANGE_H
#define LTTLWHWHT_STATE_CHANGE_H

#include "process.h"

class StateChange : public ProcessChange {
  public:
    StateChange(int pid, uint64_t timestamp):
                ProcessChange(pid, timestamp) {}
    ~StateChange() {}

    const ProcessState & getState() const { return myState; }
    StateChange * setState(const ProcessState & state) { myState = state; return this; }
  protected:
    //! Applies me to process
    void applyFurtherTo(Process & process) const override;
  private:
    ProcessState myState;
};

#endif // LTTLWHWHT_STATE_CHANGE_H

#ifndef LTTLWHWHT_NOTIFICATION_CHANGE_H
#define LTTLWHWHT_NOTIFICATION_CHANGE_H

#include "process.h"

class NotificationChange : public ProcessChange {
  public:
    NotificationChange(int pid, uint64_t timestamp):
                ProcessChange(pid, timestamp) {}
    ~NotificationChange() {}

    const ProcessState & getState() const { return myState; }
    NotificationChange * setState(const ProcessState & state) { myState = state; return this; }
  protected:
    //! Applies me to process
    void applyFurtherTo(Process & process) const override;
  private:
    ProcessState myState;
};

#endif // LTTLWHWHT_NOTIFICATION_CHANGE_H