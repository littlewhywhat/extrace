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

#ifndef LTTLWHWHT_PROCESSSTAT_H
#define LTTLWHWHT_PROCESSSTAT_H

class Entry;
class MemoryEntry;
class CpuUsageEntry;

class ProcessStat {
  public:
    ProcessStat() {}
    ProcessStat(int pid, int cpuUse, long vss, long rss, long pss, long uss):
                myPID(pid), myCpuUsage(cpuUse), myVss(vss), myRss(rss), myPss(pss), myUss(uss) {}
    void updateWith(const Entry & entry);
    void updateWith(const MemoryEntry & entry);
    void updateWith(const CpuUsageEntry & entry);
    int getPID()    const { return myPID;    }
    int getCpuUse() const { return myCpuUsage; }
    long getVss()   const { return myVss;    }
    long getRss()   const { return myRss;    }
    long getPss()   const { return myPss;    }
    long getUss()   const { return myUss;    }
    int  getTimeHigh() const { return myTimeHigh; }
    int  getTimeLow()  const { return myTimeLow; }
    ProcessStat * setPID(int pid)       { myPID    = pid;    return this; }
    ProcessStat * setCpuUsage(int cpuUse) { myCpuUsage = cpuUse; return this; }
    ProcessStat * setVss(long vss)      { myVss    = vss;    return this; }
    ProcessStat * setRss(long rss)      { myRss    = rss;    return this; }
    ProcessStat * setPss(long pss)      { myPss    = pss;    return this; }
    ProcessStat * setUss(long uss)      { myUss    = uss;    return this; }
    ProcessStat * setTimeHigh(int timeHigh) { myTimeHigh = timeHigh; return this; }
    ProcessStat * setTimeLow(int timeLow) { myTimeLow = timeLow; return this; }
   
    //! Says if my fields are all filled (not -1)
    bool isReady() const;
    //! Compares me to another. Time fields are not involved in comparance
    
  private:
    int myPID    = -1;
    int myCpuUsage = -1;
    long myVss   = -1;
    long myRss   = -1;
    long myPss   = -1;
    long myUss   = -1;
    int myTimeHigh = -1;
    int myTimeLow  = -1;
};

inline bool operator==(const ProcessStat & lhs, const ProcessStat & rhs) {
  return lhs.getPID() == rhs.getPID()    
         && lhs.getCpuUse() == rhs.getCpuUse() 
         && lhs.getVss() == rhs.getVss()   
         && lhs.getRss() == rhs.getRss()   
         && lhs.getPss() == rhs.getPss()   
         && lhs.getUss() == rhs.getUss()   
         && lhs.getTimeHigh() == rhs.getTimeHigh() 
         && lhs.getTimeLow() == rhs.getTimeLow();
}

#endif // LTTLWHWHT_PROCESSSTAT_H