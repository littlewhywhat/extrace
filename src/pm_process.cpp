#include "pm_process.h"

#include "pm_memoryusage.h"
#include "pm_workingset.h"

PM_Kernel::PM_Process * PM_Kernel::PM_Process::tryCreate(PM_Kernel & kernel, long pid) {
  auto * process = new PM_Process();
  if (!process->tryInit(kernel, pid)) {
    delete process;
    process = NULL;
  }
  return process;
}

bool PM_Kernel::PM_Process::tryInit(PM_Kernel & kernel, long pid) {
  fprintf(stderr, "called pm_process_create\n");
  int error = pm_process_create(kernel.m_Kernel, (pid_t)pid, &m_Process);
  if (error) {
    fprintf(stderr, "error creating process interface -- "
                    "does process %ld really exist?\n", pid);
    return false;
  }
  return true;
}

MemoryUsage * PM_Kernel::PM_Process::tryCreateMemoryUsage() {
  return new PM_MemoryUsage(*this);
}

MemoryUsage * PM_Kernel::PM_Process::tryCreateWorkingSet() {
  return new PM_WorkingSet(*this);
}