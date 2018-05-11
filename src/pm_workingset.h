#ifndef LTTLWHWHT_PM_WORKINGSET_H
#define LTTLWHWHT_PM_WORKINGSET_H

#include "pm_abstractmemoryusage.h"

#include "pm_process.h"

class PM_WorkingSet : public PM_Kernel::PM_Process::PM_AbstractMemoryUsage {
  public:
    PM_WorkingSet(PM_Kernel::PM_Process & process): PM_AbstractMemoryUsage(process) {}
    ~PM_WorkingSet() {}
    bool tryUpdate() override;
};

#endif // LTTLWHWHT_PM_WORKINGSET_H