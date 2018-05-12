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

#include <time.h> // nanosleep, struct timespec
#include <errno.h>

#include "pm_kernelbuilder.h"

//TODO refactor
bool MemorySampleAction::tryRun() {
  auto & traceBuffer = m_Environment->getTraceBuffer();
  bool ok = true;

  auto * kernel = PM_KernelBuilder().tryCreate();
  if (!kernel) {
    delete kernel;
    return false;
  }
  auto * process = kernel->tryCreateProcess(m_PID);
  if (!process) {
    delete process;
    delete kernel;
    return false;
  }
  auto * memoryUsage = process->tryCreateMemoryUsage();
  if (!memoryUsage) {
    delete memoryUsage;
    delete process;
    delete kernel;
    return false;
  }
  
  char buffer[128];

  struct timespec timeLeft;
  timeLeft.tv_sec = 0;
  timeLeft.tv_nsec = m_Period;
  for (uint32_t i = 0; i < m_Times; i++) {
    do {
      if (!memoryUsage->tryUpdate()) {
        delete memoryUsage;
        delete process;
        delete kernel;
        return false;
      }
      if (snprintf(buffer, 128, "VSS=%7ld  RSS=%7ld PSS=%7ld USS=%7ld PID=%d", 
               memoryUsage->getVSS(), memoryUsage->getRSS(), memoryUsage->getPSS(), memoryUsage->getUSS(),
               m_PID) == 128) {
        fprintf(m_Wire.getErrorStream(), "can't create string for buffer\n");
        delete memoryUsage;
        delete process;
        delete kernel;
        return false;
      }
      traceBuffer.tryWriteString(buffer);
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
