#ifndef LTTLWHWHT_MEMORYUSAGE_H
#define LTTLWHWHT_MEMORYUSAGE_H

#include <stdlib.h>

class MemoryUsage {
  public:
    virtual ~MemoryUsage() {}
    virtual bool tryUpdate() = 0;
    virtual size_t getVSS() const = 0;
    virtual size_t getRSS() const = 0;
    virtual size_t getPSS() const = 0;
    virtual size_t getUSS() const = 0;
};

#endif // LTTLWHWHT_MEMORYUSAGE_H