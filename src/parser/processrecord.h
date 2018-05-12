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

#ifndef LTTLWHWHT_PROCESSRECORD_H
#define LTTLWHWHT_PROCESSRECORD_H

#include "process.h"

#include <ostream>
#include <iomanip>
#include <cstdint>

using namespace std;

class ProcessRecord {
  public:
    ProcessRecord() {}
    ProcessRecord(const Process * process);
    int getPID()    const { return myPID;    }
    int getCpuUse() const { return myCpuUsage; }
    long getVss()   const { return myVss;    }
    long getRss()   const { return myRss;    }
    long getPss()   const { return myPss;    }
    long getUss()   const { return myUss;    }
    uint64_t getTimeStamp() const { return myTimeStamp; }
    ProcessRecord * setPID(int pid)       { myPID    = pid;    return this; }
    ProcessRecord * setCpuUsage(int cpuUse) { myCpuUsage = cpuUse; return this; }
    ProcessRecord * setVss(long vss)      { myVss    = vss;    return this; }
    ProcessRecord * setRss(long rss)      { myRss    = rss;    return this; }
    ProcessRecord * setPss(long pss)      { myPss    = pss;    return this; }
    ProcessRecord * setUss(long uss)      { myUss    = uss;    return this; }
    ProcessRecord * setTimeStamp(uint64_t time) { myTimeStamp = time; return this; }
    ProcessRecord * setState(const ProcessState & state) { myState = state; return this; }

    friend ostream & operator << (ostream& os, const ProcessRecord & record) {
      os << setw(5);
      if (record.myPID == -1) {
        os << "?";
      } else {
        os << record.myPID;
      }
      os << " | ";
      os << setw(3);
      if (record.myCpuUsage == -1) {
        os << "?";
      }
      else {
        os << record.myCpuUsage;
      }
      os << " %";
      os << " | ";
      os << setw(10);
      if (record.myVss == -1) {
        os << "?";
      }
      else {
        os << record.myVss;
      }
      os << " | ";
      os << setw(10);
      if (record.myRss == -1) {
        os << "?";
      }
      else {
        os << record.myRss;
      }
      os << " | ";
      os << setw(10);
      if (record.myPss == -1) {
        os << "?";
      }
      else {
        os << record.myPss;
      }
      os << " | ";
      os << setw(10);
      if (record.myUss == -1) {
        os << "?";
      }
      else {
        os << record.myUss;
      }
      os << " | ";
      os << setw(11);
      if (record.myTimeStamp == UINT64_MAX) {
        os << "?";
      }
      else {
        os << record.myTimeStamp;
      }
      os << " | ";
      switch(record.myState) {
        case ProcessState::RUNNING:
          os << "RUNNING";
          break;
        case ProcessState::AWAKE:
          os << "AWAKE";
          break;
        case ProcessState::SLEEPING:
          os << "SLEEPING";
          break;
        case ProcessState::UNKNOWN:
          os << "?";
          break;
      }
      return os;
    }    
  private:
    int myPID    = -1;
    int myCpuUsage = -1;
    long myVss   = -1;
    long myRss   = -1;
    long myPss   = -1;
    long myUss   = -1;
    // TODO better to switch to long long
    uint64_t myTimeStamp = UINT64_MAX;
    ProcessState myState = ProcessState::UNKNOWN;
};

#endif // LTTLWHWHT_PROCESSRECORD_H