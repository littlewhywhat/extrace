#ifndef LTTLWHWHT_KERNEL_H
#define LTTLWHWHT_KERNEL_H

#include "process_mem.h"

class Kernel {
  public:
    virtual ~Kernel() {}
    virtual Process * tryCreateProcess(long pid) = 0;
};

#endif // LTTLWHWHT_KERNEL_H
