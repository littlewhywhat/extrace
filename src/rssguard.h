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

#ifndef LTTLWHWHT_RSSGUARD_H
#define LTTLWHWHT_RSSGUARD_H

#include "processrecord.h"

//! I am guard that acts on ProcessRecord and determine if record RSS increase overpassed the limit
class RssGuard {
  public:
    RssGuard(uint64_t rssLimit): myRssLimit(rssLimit) {}
    //! Says if record crosses the limit for RSS relatively to history value
    bool pass(const ProcessRecord & record, uint64_t history) const;
  private:
    //! My RSS limit
    uint64_t myRssLimit = 0;
};

#endif // LTTLWHWHT_RSSGUARD_H