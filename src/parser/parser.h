#include <string>
#include <vector>
#include <map>

#ifndef PARSER_H
#define PARSER_H

using namespace std;

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

bool ProcessStat::isReady() const {
  return myPID    != -1
         && myCpuUsage != -1
         && myVss   != -1
         && myRss   != -1
         && myPss   != -1
         && myUss   != -1;
}

#include <string>

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

void Entry::update(ProcessStat & procStat) const {
  procStat.setPID(myPID);
  procStat.setTimeHigh(myTimeHigh);
  procStat.setTimeLow(myTimeLow);
  updateFurther(procStat);
}

int Entry::getPID() const {
  return myPID;
}

Entry* Entry::setPID(int pid) {
  myPID = pid;
  return this;
}

int     Entry::getTimeHigh() const {
  return myTimeHigh;
}

Entry * Entry::setTimeHigh(int time) {
  myTimeHigh = time;
  return this;
}

int     Entry::getTimeLow() const {
  return myTimeLow;
}

Entry * Entry::setTimeLow(int time) {
  myTimeLow = time;
  return this;
}

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

void MemoryEntry::updateFurther(ProcessStat & procStat) const {
  procStat.updateWith(*this);
}

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

void CpuUsageEntry::updateFurther(ProcessStat & procStat) const {
  procStat.updateWith(*this);
}

void ProcessStat::updateWith(const Entry & entry) {
  printf("error: not supported entry type\n");
}

void ProcessStat::updateWith(const MemoryEntry & entry) {
  myVss   = entry.getVss();
  myRss   = entry.getRss();
  myPss   = entry.getPss();
  myUss   = entry.getUss();
}

void ProcessStat::updateWith(const CpuUsageEntry & entry) {
  myCpuUsage = entry.getCpuUsage();
}

class EntryFile {
  public:
    virtual ~EntryFile() {}
    virtual void parseTo(vector<Entry*> & entries) const = 0;
};

//! I am EntryFile creator
class EntryFileCreator {
  public:
    virtual ~EntryFileCreator() {}
    virtual EntryFile * create(const string & filename) const = 0;
};
//! I am an extrace trace file. I can be parsed to vector of ProcessStat.
class ExtraceFile {
  public:
    ExtraceFile(const string & filename,
                EntryFileCreator * fileCreator):
                myFilename(filename),
                myEntryFileCreator(fileCreator) {}
    // TODO destructor
    bool tryParseTo(vector<ProcessStat> & procStats) const;
  private:
    const string & myFilename;
    EntryFileCreator * myEntryFileCreator = NULL;
};

bool ExtraceFile::tryParseTo(vector<ProcessStat> & procStats) const {
  EntryFile * entriesFile = myEntryFileCreator->create(myFilename);

  vector<Entry*> entries;
  entriesFile->parseTo(entries);
  // NOTE some events - like switch could be relevant to two process - switched from and switched to
  // TODO Insert another layer to convert entries to memory and cpu usage ... somehow
  map<int, ProcessStat> procStatsPerPid;
  for (auto * entry : entries) {
    if (procStatsPerPid.find(entry->getPID()) == procStatsPerPid.end()) {
      ProcessStat procStat;
      procStat.setPID(entry->getPID());
      procStatsPerPid[entry->getPID()] = procStat;
    }
    auto & procStat = procStatsPerPid[entry->getPID()];
    // double dispatch - could be MemoryEntry, Switch Entry and so on
    // update is virtual for ExtraceFileEntry and they call procState.updateWith that is overloaded
    entry->update(procStat);
    // if all fields are filled
    if (procStat.isReady()) {
      procStats.push_back(procStat);
    }
  }
  return true;
}

#endif // PARSER_H