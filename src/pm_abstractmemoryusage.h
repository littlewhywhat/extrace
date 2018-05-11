#ifndef LTTLWHWHT_PM_ABSTRACEMEMORYUSAGE_H
#define LTTLWHWHT_PM_ABSTRACEMEMORYUSAGE_H

#include "memoryusage.h"

#include "pm_process.h"

class PM_Kernel::PM_Process::PM_AbstractMemoryUsage: public MemoryUsage {
  public:
    PM_AbstractMemoryUsage(PM_Kernel::PM_Process & process):
                           m_Process(process.m_Process) {};
    ~PM_AbstractMemoryUsage() {}
    virtual bool tryUpdate();
    size_t getVSS() const override;
    size_t getRSS() const override;
    size_t getPSS() const override;
    size_t getUSS() const override;
  protected:
    pm_process_t * m_Process;
    pm_memusage_t m_MemUsage;
};

#endif // LTTLWHWHT_PM_ABSTRACEMEMORYUSAGE_H