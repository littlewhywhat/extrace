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

#include "ussfilter.h"

void USSFilter::filter(queue<ProcessRecord*> & records) const {
  size_t size = records.size();
  uint64_t history;
  bool initHistory = false;
  for (size_t i = 0; i < size; i++) {
    auto * record = records.front();
    records.pop();
    if (record->hasUss()) {
      if (!initHistory) {
        history = record->getUss();
        records.push(record);
        initHistory = true;
      }
      else {
        uint64_t rss = record->getUss();
        if (rss < history + myUssLimit) {
          continue;
        }
        records.push(record);
      }
    }
  }
}