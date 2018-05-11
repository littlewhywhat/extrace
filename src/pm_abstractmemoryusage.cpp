#include "pm_abstractmemoryusage.h"

size_t PM_Kernel::PM_Process::PM_AbstractMemoryUsage::getVSS() const {
  return m_MemUsage.vss;
}

size_t PM_Kernel::PM_Process::PM_AbstractMemoryUsage::getRSS() const {
  return m_MemUsage.rss;
}

size_t PM_Kernel::PM_Process::PM_AbstractMemoryUsage::getPSS() const {
  return m_MemUsage.pss;
}

size_t PM_Kernel::PM_Process::PM_AbstractMemoryUsage::getUSS() const {
  return m_MemUsage.uss;
}
