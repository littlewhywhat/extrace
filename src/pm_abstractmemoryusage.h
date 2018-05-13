#ifndef LTTLWHWHT_PM_ABSTRACEMEMORYUSAGE_H
#define LTTLWHWHT_PM_ABSTRACEMEMORYUSAGE_H

#include "memoryusage.h"

#include "pm_process.h"

class PM_Kernel::PM_Process::PM_AbstractMemoryUsage: public MemoryUsage {
  public:
    PM_AbstractMemoryUsage(PM_Kernel::PM_Process & process):
                           myPID(process.m_PID), m_Process(process.m_Process) {};
    ~PM_AbstractMemoryUsage() {}
    virtual bool tryUpdate();
    int getPID() const override;
    uint64_t getVSS() const override;
    uint64_t getRSS() const override;
    uint64_t getPSS() const override;
    uint64_t getUSS() const override;
  protected:
    int myPID;
    pm_process_t * m_Process;
    pm_memusage_t m_MemUsage;
};

#endif // LTTLWHWHT_PM_ABSTRACEMEMORYUSAGE_H