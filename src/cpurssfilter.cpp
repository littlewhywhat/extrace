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

#include "cpurssfilter.h"

void CpuRssFilter::process(queue<ProcessRecord *> & records) const {
  bool rssInit = false;
  uint64_t initRssVal = 0;
  size_t size = records.size();
  for (size_t i = 0; i < size; i++) {
    auto * record = records.front();
    records.pop();
    // if rss is not init, init 
    // and record will pass RssGuard in any case
    if (!rssInit) {
      if (record->hasRss()) {
        initRssVal = record->getRss();
        rssInit = true;
        records.push(record);
        continue;
      }
    }
    // if process had record with rss
    // it should all others with rss
    if (rssInit && !record->hasRss()) {
      fprintf(stderr, "error filter found broken record with no Rss though previous have\n");
      continue;
    }
    if (rssInit && myRssGuard->pass(*record, initRssVal)) {
      records.push(record);
      continue;
    }
    // here either rssInit is false and 
    // record has no rss value
    // or rss doesn't pass RssGuard
    if (record->hasCpuUse()) {
      if (myCpuGuard->pass(*record)) {
        records.push(record);
        continue;
      }
    }

  }
}