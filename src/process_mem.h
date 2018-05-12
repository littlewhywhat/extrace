#ifndef LTTLWHWHT_PROCESS_H
#define LTTLWHWHT_PROCESS_H

#include "memoryusage.h"

class Process {
  public:
    virtual ~Process() {}
    virtual MemoryUsage * tryCreateMemoryUsage() = 0;
    virtual MemoryUsage * tryCreateWorkingSet() = 0;
};

#endif // LTTLWHWHT_PROCESS_H