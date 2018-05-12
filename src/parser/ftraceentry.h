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
// TODO separate to different files

#ifndef LTTLWHWHT_FTRACE_ENTRY_H
#define LTTLWHWHT_FTRACE_ENTRY_H

//! I am entry
class FTraceEntry {
  public:
    FTraceEntry(int pid, int timeLow, int timeHigh):
              myPID(pid), myTimeLow(timeLow), myTimeHigh(timeHigh) {}
    virtual ~FTraceEntry() {}
    int getPID()      const { return myPID; }
    int getTimeHigh() const { return myTimeHigh; }
    int getTimeLow()  const { return myTimeLow; }

    FTraceEntry * setPID(int pid)       { myPID = pid;       return this; }
    FTraceEntry * setTimeHigh(int time) { myTimeHigh = time; return this; }
    FTraceEntry * setTimeLow(int time)  { myTimeLow = time;  return this; }
  protected:
    int myPID;
    int myTimeLow;
    int myTimeHigh;
};

#endif // LTTLWHWHT_FTRACE_ENTRY_H

#ifndef LTTLWHWHT_SCHED_WAKEUP_ENTRY_H
#define LTTLWHWHT_SCHED_WAKEUP_ENTRY_H

#include <string>

using namespace std;

class SchedWakeUpEntry : public FTraceEntry {
  public:
    SchedWakeUpEntry(int pid, int timeLow, int timeHigh):
                      FTraceEntry(pid, timeLow, timeHigh) {}
    const string & getCommandName() { return myCommandName; }
    int getWakedUpPID() { return myWakedUpPID; }
    int getPriority() { return myPriority; }
    bool getSuccess() { return mySuccess; }
    int getTargetCPU() { return myTargetCPU; }

    SchedWakeUpEntry * setCommandName(const string & commandName) { myCommandName = commandName; return this; }
    SchedWakeUpEntry * setWakedUpPID(int wakedUpPID) { myWakedUpPID = wakedUpPID; return this; }
    SchedWakeUpEntry * setPriority(int priority) { myPriority = priority; return this; }
    SchedWakeUpEntry * setSuccess(bool success) { mySuccess = success; return this; }
    SchedWakeUpEntry * setTargetCPU(int targetCPU) { myTargetCPU = targetCPU; return this; }
  private:
    string myCommandName;
    int    myWakedUpPID;
    int    myPriority;
    bool   mySuccess;
    int    myTargetCPU;
};

#endif // LTTLWHWHT_SCHED_WAKEUP_ENTRY_H

#ifndef LTTLWHWHT_SCHED_SWITCH_ENTRY_H
#define LTTLWHWHT_SCHED_SWITCH_ENTRY_H

#include <string>

using namespace std;

class SchedSwitchEntry : public FTraceEntry {
  public:
    SchedSwitchEntry(int pid, int timeLow, int timeHigh):
                      FTraceEntry(pid, timeLow, timeHigh) {}
    const string & getPrevCommandName() const { return myPrevCommandName; }
    int            getPrevPriority() const { return myPrevPriority; }
    int            getPrevPID() const { return myPrevPID; }
    const string & getNextCommandName() const { return myNextCommandName; }
    int            getNextPriority() const { return myNextPriority; }
    int            getNextPID() const { return myNextPID; }

    SchedSwitchEntry * setPrevCommandName(const string & prevCommandName) { myPrevCommandName = prevCommandName; return this; }
    SchedSwitchEntry * setPrevPriority(int prevPriority) { myPrevPriority = prevPriority; return this; }
    SchedSwitchEntry * setPrevPID(int prevPID) { myPrevPID = prevPID; return this; }
    SchedSwitchEntry * setNextCommandName(const string & nextCommandName) { myNextCommandName = nextCommandName; return this; }
    SchedSwitchEntry * setNextPriority(int nextPriority) { myNextPriority = nextPriority; return this; }
    SchedSwitchEntry * setNextPID(int nextPID) { myNextPID = nextPID; return this; }
  private:
    string myPrevCommandName;
    int    myPrevPriority;
    int    myPrevPID;
    string myNextCommandName;
    int    myNextPriority;
    int    myNextPID;
};

#endif // LTTLWHWHT_SCHED_SWITCH_ENTRY_H

#ifndef LTTLWHWHT_TRACING_MARK_ENTRY_H
#define LTTLWHWHT_TRACING_MARK_ENTRY_H

#include <string>

using namespace std;

class TracingMarkEntry : public FTraceEntry {
  public:
    TracingMarkEntry(int pid, int timeLow, int timeHigh):
                      FTraceEntry(pid, timeLow, timeHigh) {}
    long getVSS() const { return myVSS; }
    long getRSS() const { return myRSS; }
    long getPSS() const { return myPSS; }
    long getUSS() const { return myUSS; }
    int  getTracedPID() const { return myTracedPID; }

    TracingMarkEntry * setVSS(long vss) { myVSS = vss; return this; }
    TracingMarkEntry * setRSS(long rss) { myRSS = rss; return this; }
    TracingMarkEntry * setPSS(long pss) { myPSS = pss; return this; }
    TracingMarkEntry * setUSS(long uss) { myUSS = uss; return this; }
    TracingMarkEntry * setTracedPID(int pid)  { myTracedPID = pid; return this; }
  private:
    long myVSS;
    long myRSS;
    long myPSS;
    long myUSS;
     int myTracedPID;
};

#endif // LTTLWHWHT_TRACING_MARK_ENTRY_H