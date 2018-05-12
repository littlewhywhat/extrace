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

#include "tracingmarkentrycreator.h"

TracingMarkEntry * TracingMarkEntryCreator::create(int pid, long timeLow, long timeHigh,
                                              const char * content) const {
  int tracedPID;
  long vss, rss, pss, uss;
  int numArgs = 5;
  if (sscanf(content, "VSS=%ld RSS=%ld PSS=%ld USS=%ld PID=%d",
                       &vss, &rss, &pss, &uss, &tracedPID) != numArgs) {
    fprintf(stderr, "error can't parse content\n");
    return NULL;
  }
  return (new TracingMarkEntry(pid, timeLow, timeHigh))
              ->setVSS(vss)
              ->setRSS(rss)
              ->setPSS(pss)
              ->setUSS(uss)
              ->setTracedPID(tracedPID);
}
