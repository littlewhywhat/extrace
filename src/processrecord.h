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

#include "processstate.h"
#include "process.h"

#include <ostream>
#include <iomanip>
#include <cstdint>
#include <string>

using namespace std;

class ProcessRecord {
  public:
    ProcessRecord() {}
    int getPID()    const { return myPID;    }
    int getCpuUse() const { return myCpuUsage; }
    uint64_t getVss()   const { return myVss;    }
    uint64_t getRss()   const { return myRss;    }
    uint64_t getPss()   const { return myPss;    }
    uint64_t getUss()   const { return myUss;    }
    uint64_t getTimeStamp() const { return myTimeStamp; }
    ProcessState getState() const { return myState; }
    const string & getCause() const { return myCause; }

    bool hasPID()    const { return myPID != -1;    }
    bool hasCpuUse() const { return myCpuUsage != -1; }
    bool hasVss()   const { return myVss != UINT64_MAX;    }
    bool hasRss()   const { return myRss != UINT64_MAX;    }
    bool hasPss()   const { return myPss != UINT64_MAX;    }
    bool hasUss()   const { return myUss != UINT64_MAX;    }
    bool hasTimeStamp() const { return myTimeStamp != UINT64_MAX; }
    bool hasState() const { return myState != ProcessState::UNKNOWN; }
    bool hasCause() const { return !myCause.empty(); }

    ProcessRecord * setPID(int pid)       { myPID    = pid;    return this; }
    ProcessRecord * setCpuUsage(int cpuUse) { myCpuUsage = cpuUse; return this; }
    ProcessRecord * setVss(uint64_t vss)      { myVss    = vss;    return this; }
    ProcessRecord * setRss(uint64_t rss)      { myRss    = rss;    return this; }
    ProcessRecord * setPss(uint64_t pss)      { myPss    = pss;    return this; }
    ProcessRecord * setUss(uint64_t uss)      { myUss    = uss;    return this; }
    ProcessRecord * setTimeStamp(uint64_t time) { myTimeStamp = time; return this; }
    ProcessRecord * setState(const ProcessState & state) { myState = state; return this; }
    ProcessRecord * setCause(const string & cause) { myCause = cause; return this; }

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
      if (record.myVss == UINT64_MAX) {
        os << "?";
      }
      else {
        os << record.myVss / 1024;
      }
      os << " K" << " | ";
      os << setw(10);
      if (record.myRss == UINT64_MAX) {
        os << "?";
      }
      else {
        os << record.myRss / 1024;
      }
      os << " K" << " | ";
      os << setw(10);
      if (record.myPss == UINT64_MAX) {
        os << "?";
      }
      else {
        os << record.myPss / 1024;
      }
      os << " K" << " | ";
      os << setw(10);
      if (record.myUss == UINT64_MAX) {
        os << "?";
      }
      else {
        os << record.myUss / 1024;
      }
      os << " K" << " | ";
      os << setw(11);
      if (record.myTimeStamp == UINT64_MAX) {
        os << "?";
      }
      else {
        os << record.myTimeStamp;
      }
      os << " | ";
      os << setw(8);
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
      os << " |";
      if (!record.myCause.empty()) {
        os << " " << record.myCause;
      }
      return os;
    }   
  private:
    int myPID        = -1;
    int myCpuUsage   = -1;
    uint64_t myVss   = UINT64_MAX;
    uint64_t myRss   = UINT64_MAX;
    uint64_t myPss   = UINT64_MAX;
    uint64_t myUss   = UINT64_MAX;
    // TODO better to switch to long long
    uint64_t myTimeStamp = UINT64_MAX;
    ProcessState myState = ProcessState::UNKNOWN;
    string myCause;
};

inline bool operator==(const ProcessRecord & lhs, const ProcessRecord & rhs) {
  return lhs.getPID() == rhs.getPID()    
         && lhs.getCpuUse() == rhs.getCpuUse() 
         && lhs.getVss() == rhs.getVss()   
         && lhs.getRss() == rhs.getRss()   
         && lhs.getPss() == rhs.getPss()   
         && lhs.getUss() == rhs.getUss()   
         && lhs.getTimeStamp() == rhs.getTimeStamp() 
         && lhs.getState() == rhs.getState();
}

#endif // LTTLWHWHT_PROCESSRECORD_H