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

#include "processstat.h"

#include "entry.h"

#include <cstdio>

bool ProcessStat::isReady() const {
  return myPID    != -1
         && myCpuUsage != -1
         && myVss   != -1
         && myRss   != -1
         && myPss   != -1
         && myUss   != -1;
}

void ProcessStat::updateWith(const Entry & entry) {
  fprintf(stderr, "error: not supported entry type\n");
}

void ProcessStat::updateWith(const MemoryEntry & entry) {
  myVss   = entry.getVss();
  myRss   = entry.getRss();
  myPss   = entry.getPss();
  myUss   = entry.getUss();
}

void ProcessStat::updateWith(const CpuUsageEntry & entry) {
  myCpuUsage = entry.getCpuUsage();
}
