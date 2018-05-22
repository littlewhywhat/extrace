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
#include <string.h>

#include "schedswitchentrycreator.h"

SchedSwitchEntry * SchedSwitchEntryCreator::create(int pid, long timeLow, long timeHigh,
                                        const char * content) const {
  char contentBuffer[128];
  if (strlen(content) >= 128) {
    fprintf(stderr, "error too big content\n");
    return NULL;
  }
  strncpy(contentBuffer, content, 128);
  // skip 'prev_comm=
  char * prevComm = contentBuffer + 10; 
  char * prevCommInfo = strstr(prevComm, "prev_pid=");
  prevComm[prevCommInfo-prevComm-1] = '\0';

  // find 'next_comm='
  char * nextComm = strstr(prevCommInfo, "==> next_comm=");
  prevCommInfo[nextComm-prevCommInfo-1] = '\0';

  // skip '==> next_comm='
  nextComm += 14; 
  char * nextCommInfo = strstr(nextComm, "next_pid=");
  nextComm[nextCommInfo-nextComm-1] = '\0';

  // parse prev info
  int prevPID, prevPrio;
  char prevState[3];
  if (sscanf(prevCommInfo, "prev_pid=%d prev_prio=%d prev_state=%2s",
                            &prevPID, &prevPrio, prevState) != 3) {
    fprintf(stderr, "error can't parse prev info content\n");
    return NULL;
  }

  // parse next info
  int nextPID, nextPrio;
  if (sscanf(nextCommInfo, "next_pid=%d next_prio=%d",
                            &nextPID, &nextPrio) != 2) {
    fprintf(stderr, "error can't parse next info content\n");
    return NULL;
  }
  return (new SchedSwitchEntry(pid, "SchedSwitch", timeLow, timeHigh))
          ->setPrevCommandName(prevComm)
          ->setPrevPriority(prevPrio)
          ->setPrevPID(prevPID)
          ->setNextCommandName(nextComm)
          ->setNextPriority(nextPrio)
          ->setNextPID(nextPID);
}