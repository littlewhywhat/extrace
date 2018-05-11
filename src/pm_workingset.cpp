#include "pm_workingset.h"

bool PM_WorkingSet::tryUpdate() {
  fprintf(stderr, "called pm_process_workingset\n");
  int error = pm_process_workingset(m_Process, &m_MemUsage, 0);
  if (error) {
    fprintf(stderr, "error getting working set for process.\n");
    return false;
  }
  return true;
}