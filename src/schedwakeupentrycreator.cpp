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

#include "schedwakeupentrycreator.h"

#include <cstring>

SchedWakeUpEntry * SchedWakeUpEntryCreator::create(int pid, long timeLow, long timeHigh,
                                                   const char * content) const {
  char contentBuffer[128];
  if (strlen(content) >= 128) {
    fprintf(stderr, "error too big content\n");
    return NULL;
  }
  strncpy(contentBuffer, content, 128);
  // skip 'comm='
  char * comm = contentBuffer + 5;
  char * commInfo = strstr(comm, "pid=");
  comm[commInfo-comm-1] = '\0';

  int wakedUpPID, prio, success, targetCpu;
  int numArgs = 4;
  if (sscanf(commInfo, "pid=%d prio=%d success=%d target_cpu=%d",
             &wakedUpPID, &prio, &success, &targetCpu) != numArgs) {
    fprintf(stderr, "error can't parse comm info\n");
    return NULL;
  }
  return (new SchedWakeUpEntry(pid, "SchedWakeup", timeLow, timeHigh))
          ->setCommandName(comm)
          ->setWakedUpPID(wakedUpPID)
          ->setPriority(prio)
          ->setSuccess(success)
          ->setTargetCPU(targetCpu);
}