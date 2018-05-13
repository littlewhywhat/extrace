#include "pm_abstractmemoryusage.h"

uint64_t PM_Kernel::PM_Process::PM_AbstractMemoryUsage::getVSS() const {
  return m_MemUsage.vss;
}

uint64_t PM_Kernel::PM_Process::PM_AbstractMemoryUsage::getRSS() const {
  return m_MemUsage.rss;
}

uint64_t PM_Kernel::PM_Process::PM_AbstractMemoryUsage::getPSS() const {
  return m_MemUsage.pss;
}

uint64_t PM_Kernel::PM_Process::PM_AbstractMemoryUsage::getUSS() const {
  return m_MemUsage.uss;
}
