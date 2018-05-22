/*
 * Copyright (C) 2018 Roman Vaivod
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "memorysampleaction.h"

#include <time.h>     // nanosleep, struct timespec
#include <errno.h>
#include <inttypes.h> // PRId64

#include "pm_kernelbuilder.h"

MemorySampleAction::~MemorySampleAction() {
  for (auto * memoryUsage : myMemoryUsages) {
    delete memoryUsage;
  }
  for (auto * process : myProcesses) {
    delete process;
  }
  delete m_Kernel;
}

bool MemorySampleAction::tryRun() {
  auto & traceBuffer = m_Environment->getTraceBuffer();
  bool ok = true;

  m_Kernel = PM_KernelBuilder().tryCreate();
  if (!m_Kernel) {
    return false;
  }
  for (int pid : myPIDs) {
    auto * process = m_Kernel->tryCreateProcess(pid);
    if (!process) {
      return false;
    }
    myProcesses.push_back(process);
    auto * memoryUsage = process->tryCreateMemoryUsage();
    if (!memoryUsage) {
      return false;
    }
    myMemoryUsages.push_back(memoryUsage);
  }
  
  char printBuffer[128];

  struct timespec timeLeft;
  timeLeft.tv_sec = 0;
  timeLeft.tv_nsec = m_Period;
  for (uint32_t i = 0; (i < m_Times) && !m_Signal.isFired(); i++) {
    do {
      for (auto * memoryUsage : myMemoryUsages) {
        if (!memoryUsage->tryUpdate()) {
          return false;
        }
        if (snprintf(printBuffer, 128, "M: VSS=%7" PRId64 " RSS=%7" PRId64 " PSS=%7" PRId64 " USS=%7" PRId64 " PID=%d", 
                 memoryUsage->getVSS(), memoryUsage->getRSS(), memoryUsage->getPSS(), memoryUsage->getUSS(),
                 memoryUsage->getPID()) == 128) {
          fprintf(m_Wire.getErrorStream(), "can't create string for buffer\n");
          return false;
        }
        traceBuffer.tryWriteString(printBuffer);
      }
      if (m_Signal.isFired()) {
        ok = false;
        break;
      }
     } while (nanosleep(&timeLeft, &timeLeft) == -1 && errno == EINTR);
  }
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error MemorySampleAction::tryRun - sleep aborted\n");
  }
  return ok;
}
