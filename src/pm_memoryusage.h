#ifndef LTTLWHWHT_PM_MEMORYUSAGE_H
#define LTTLWHWHT_PM_MEMORYUSAGE_H

#include "pm_abstractmemoryusage.h"

#include "pm_process.h"

class PM_MemoryUsage : public PM_Kernel::PM_Process::PM_AbstractMemoryUsage {
  public:
    PM_MemoryUsage(PM_Kernel::PM_Process & process): PM_AbstractMemoryUsage(process) {}
    ~PM_MemoryUsage() {}
    bool tryUpdate() override;
};

#endif // LTTLWHWHT_PM_MEMORYUSAGE_H