#include "pm_kernel.h"
#include "pm_process.h"

Process * PM_Kernel::tryCreateProcess(long pid) {
  return PM_Kernel::PM_Process::tryCreate(*this, pid);
}

Kernel * PM_Kernel::tryCreate() {
  auto * kernel = new PM_Kernel();
  if (!kernel->tryInit()) {
    delete kernel;
    kernel = NULL;
  }
  return kernel;
}

bool PM_Kernel::tryInit() {
  int error = pm_kernel_create(&m_Kernel);
  if (error) {
    fprintf(stderr, "error creating kernel interface -- "
                    "does this kernel have pagemap?\n");
    return false;
  }
  return true;
}
