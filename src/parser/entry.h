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

#ifndef LTTLWHWHT_ENTRY_H
#define LTTLWHWHT_ENTRY_H

#include "processstat.h"

//! I am entry
class Entry {
  public:
    Entry(int pid, int timeLow, int timeHigh):
              myPID(pid), myTimeLow(timeLow), myTimeHigh(timeHigh) {}
    virtual ~Entry() {}
    int     getPID() const;
    Entry * setPID(int pid);
    int     getTimeHigh() const;
    Entry * setTimeHigh(int time);
    int     getTimeLow() const;
    Entry * setTimeLow(int time);
    void update(ProcessStat & procStat) const;
  protected:
    virtual void updateFurther(ProcessStat & procStat) const = 0;
    int myPID = -1;
    int myTimeLow = -1;
    int myTimeHigh = -1;
};

#endif // LTTLWHWHT_ENTRY_H

#ifndef LTTLWHWHT_MEMORY_ENTRY_H
#define LTTLWHWHT_MEMORY_ENTRY_H

//! I am memory entry
class MemoryEntry : public Entry {
  public:
    MemoryEntry(int pid, int timeLow, int timeHigh): Entry(pid, timeLow, timeHigh) {}
    long getVss() const { return myVss; }
    MemoryEntry * setVss(long vss) { myVss = vss; return this; }
    long getRss() const { return myRss; }
    MemoryEntry * setRss(long rss) { myRss = rss; return this; }
    long getPss() const { return myPss; }
    MemoryEntry * setPss(long pss) { myPss = pss; return this; }
    long getUss() const { return myUss; }
    MemoryEntry * setUss(long uss) { myUss = uss; return this; }
  protected:
    void updateFurther(ProcessStat & procStat) const override;
  private:
    long myVss = -1;
    long myRss = -1;
    long myPss = -1;
    long myUss = -1;
};

#endif // LTTLWHWHT_MEMORY_ENTRY_H

#ifndef LTTLWHWHT_CPUUSAGE_ENTRY_H
#define LTTLWHWHT_CPUUSAGE_ENTRY_H

//! I am switch entry
class CpuUsageEntry: public Entry {
  public:
    CpuUsageEntry(int pid, int timeLow, int timeHigh): Entry(pid, timeLow, timeHigh) {}
    int getCpuUsage() const { return myCpuUsage; }
    CpuUsageEntry * setCpuUsage(long cpuUsage) { myCpuUsage = cpuUsage; return this; }
  protected:
    void updateFurther(ProcessStat & procStat) const override;
  private:
    int myCpuUsage = -1; 
};

#endif // LTTLWHWHT_CPUUSAGE_ENTRY_H
