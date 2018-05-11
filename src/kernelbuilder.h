#ifndef LTTLWHWHT_KERNELBUILDER_H
#define LTTLWHWHT_KERNELBUILDER_H

#include "kernel.h"

class KernelBuilder {
  public:
    virtual ~KernelBuilder() {}
    virtual Kernel * tryCreate() = 0;
};

#endif // LTTLWHWHT_KERNELBUILDER_H