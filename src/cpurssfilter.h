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

#ifndef LTTLWHWHT_CPURSSFILTER_H
#define LTTLWHWHT_CPURSSFILTER_H

#include <queue>

#include "processrecord.h"
#include "cpuguard.h"
#include "rssguard.h"

using namespace std;

//! I am a filter of process records. I can filter a queue of records
class CpuRssFilter {
  public:
    CpuRssFilter(const CpuGuard * cpuGuard,
                 const RssGuard * rssGuard):
                 myCpuGuard(cpuGuard),
                 myRssGuard(rssGuard) {}
    ~CpuRssFilter() {
      delete myCpuGuard;
      delete myRssGuard;
    }
    //! Filters records that doesn't pass my guards
    void process(queue<ProcessRecord *> & records) const;
  private:
    //! My CpuGuard
    const CpuGuard * myCpuGuard;
    //! My RssGuard
    const RssGuard * myRssGuard;
};

#endif // LTTLWHWHT_CPURSSFILTER_H