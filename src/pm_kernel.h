#ifndef LTTLWHWHT_PM_KERNEL_H
#define LTTLWHWHT_PM_KERNEL_H

#include "kernel.h"

#include "pagemap/pagemap.h"

class PM_Kernel : public Kernel {
  public:
    Process * tryCreateProcess(long pid) override;
    static Kernel * tryCreate();
    class PM_Process;
  private:
    PM_Kernel() {}
    bool tryInit();
    pm_kernel_t *m_Kernel;
};

#endif // LTTLWHWHT_PM_KERNEL_H