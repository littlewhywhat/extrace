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

#include "schedswitchentrycreator.h"

SchedSwitchEntry * SchedSwitchEntryCreator::create(int pid, long timeLow, long timeHigh,
                                        const char * content) const {
  int prevPID, nextPID, prevPrio, nextPrio;
  char prevState[3];
  char prevComm[128], nextComm[128];
  int numArgs = 7;
  if (sscanf(content, "prev_comm=%s prev_pid=%d prev_prio=%d prev_state=%2s"
                      " ==> next_comm=%s next_pid=%d next_prio=%d",
                       prevComm, &prevPID, &prevPrio, prevState,
                       nextComm, &nextPID, &nextPrio) != numArgs) {
    fprintf(stderr, "error can't parse content\n");
    return NULL;
  }
  return (new SchedSwitchEntry(pid, timeLow, timeHigh))
          ->setPrevCommandName(prevComm)
          ->setPrevPriority(prevPrio)
          ->setPrevPID(prevPID)
          ->setNextCommandName(nextComm)
          ->setNextPriority(nextPrio)
          ->setNextPID(nextPID);
}