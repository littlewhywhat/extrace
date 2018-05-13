#ifndef LTTLWHWHT_MEMORYUSAGE_H
#define LTTLWHWHT_MEMORYUSAGE_H

#include <stdlib.h>
#include <stdint.h>

class MemoryUsage {
  public:
    virtual ~MemoryUsage() {}
    virtual bool tryUpdate() = 0;
    virtual uint64_t getVSS() const = 0;
    virtual uint64_t getRSS() const = 0;
    virtual uint64_t getPSS() const = 0;
    virtual uint64_t getUSS() const = 0;
};

#endif // LTTLWHWHT_MEMORYUSAGE_H