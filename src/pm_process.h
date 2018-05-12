#ifndef LTTLWHWHT_PM_PROCESS_H
#define LTTLWHWHT_PM_PROCESS_H

#include "process_mem.h"

#include "pm_kernel.h"

#include "pagemap/pagemap.h"

class PM_Kernel::PM_Process : public Process {
  public:
    static PM_Process * tryCreate(PM_Kernel & kernel, long pid);
    MemoryUsage * tryCreateMemoryUsage() override;
    MemoryUsage * tryCreateWorkingSet() override;
    class PM_AbstractMemoryUsage;
  private:
    PM_Process() {}
    bool tryInit(PM_Kernel & kernel, long pid);
    pm_process_t * m_Process;
};

#endif // LTTLWHWHT_PM_PROCESS_H