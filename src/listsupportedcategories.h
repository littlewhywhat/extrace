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

#ifndef LTTLWHWHT_LISTSUPPORTEDCATEGORIES_H
#define LTTLWHWHT_LISTSUPPORTEDCATEGORIES_H

#include "traceaction.h"
#include "kerneltracesystem.h"
#include "android.h"
#include "androidtracecategory.h"
#include "kerneltracecategory.h"

class ListSupportedCategories : public TraceAction {
  public:
    ListSupportedCategories(const Wire & wire, 
                            const shared_ptr<Trace> & trace);
    ~ListSupportedCategories();
    bool tryRun() override;
  private:
    map<KernelTraceSystem::TraceCategory, KernelTraceCategory *> m_KernelTraceCategories;
    map<Android::TraceCategory, AndroidTraceCategory *> m_AndroidTraceCategories;
};

#endif // LTTLWHWHT_LISTSUPPORTEDCATEGORIES_H