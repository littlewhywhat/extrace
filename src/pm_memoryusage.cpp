#include "pm_memoryusage.h"

bool PM_MemoryUsage::tryUpdate() {
  int error = pm_process_usage(m_Process, &m_MemUsage);
  if (error) {
    fprintf(stderr, "error getting usage for process.\n");
    return false;
  }
  return true;
}