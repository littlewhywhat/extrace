#ifndef LTTLWHWHT_PM_KERNELBUILDER_H
#define LTTLWHWHT_PM_KERNELBUILDER_H

#include "kernelbuilder.h"

class PM_KernelBuilder : public KernelBuilder 
{
  public:
    ~PM_KernelBuilder() {}
    Kernel * tryCreate() override;
};

#endif // LTTLWHWHT_PM_KERNELBUILDER_H