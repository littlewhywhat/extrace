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

#ifndef LTTLWHWHT_KERNELTRACESYSTEM_H
#define LTTLWHWHT_KERNELTRACESYSTEM_H

#include <set>
#include <string>
#include <vector>

#include "ftrace.h"

using namespace std;

//! I am a kernel trace system that controls what is traced in kernel

//! I manage what categories or functions are traced in kernel
class KernelTraceSystem : public Wired {
  public:
    enum class TraceCategory;
    //! Initializes category groups
    KernelTraceSystem(const Wire & wire, FTrace * ftrace);
    //! Deletes m_FTrace
    ~KernelTraceSystem();
    //! Tries to enable trace category 
    bool tryEnableCategory(const TraceCategory & traceCategory); 
    //! Tries to disable all set categories
    bool tryDisableAllCategories();
    //! Tries set specified function to trace
    bool trySetFunctions(const set<string> functions); 
    //! Tries disable all set functions
    bool tryDisableAllFunctions();
    //! Says if specified category is supported without root privelege
    bool supportsCategory(const TraceCategory & traceCategory) const;
  private:
    //! I am a member of trace category optional or required to enable it.

    //! Group of us define trace category
    class TraceCategoryMember {
      public:
        TraceCategoryMember(const FTrace::TracePoint & tracePoint, bool isRequired):
                            m_TracePoint(tracePoint), m_IsRequired(isRequired) {}
        const FTrace::TracePoint & getTracePoint() const { return m_TracePoint; }
        const bool & isRequired() const { return m_IsRequired; }
      private:
        FTrace::TracePoint m_TracePoint;
        bool m_IsRequired;
    };
    //! Definitions of each TraceCategory by groups of TraceCategoryMembers
    map<TraceCategory, vector<TraceCategoryMember>> m_TraceCategoryGroups;
    //! Pointer to FTrace
    FTrace * m_FTrace = NULL;
    //! Tries to disable group of TraceCategoryMembers
    
    //! This method assumes that corresponding TraceCategory is supported.
    //! Returns true if all required members were successfully disabled.
    //! Returns false if at least on required member was not disabled.
    bool tryDisableCategoryGroup(const vector<TraceCategoryMember> & categoryGroup);
    //! Tries to enable group of TraceCategoryMembers
    
    //! This method assumes that corresponding TraceCategory is supported.
    //! Returns true if all required members were successfully enabled.
    //! Returns false if at least on required member was not enabled.
    bool tryEnableCategoryGroup(const vector<TraceCategoryMember> & categoryGroup);
    //! Says if specified category is supported with root privelege
    bool supportsCategoryWithRoot(const TraceCategory & traceCategory) const;
    //! Verifies if specified functions are set for tracing
    bool verifyFunctions(const set<string> & functions) const;
};

//! Category of kernel events to trace
enum class KernelTraceSystem::TraceCategory {
  SCHED,         //!< includes processes scheduling 
  IRQ,           //!< includes IRQ events
  FREQ,          //!< includes frequencies information
  MEMBUS,        //!< includes memory bus utilization
  IDLE,          //!< includes CPU idle events
  DISK,          //!< includes disk operations
  MMC,           //!< TODO
  LOAD,          //!< TODO
  SYNC,          //!< TODO
  WORKQUEUE,     //!< TODO
  MEMRECLAIM,    //!< TODO
  REGULATOR,     //!< TODO
  BINDER_DRIVER, //!< TODO
  BINDER_LOCK,   //!< TODO
  PAGECACHE,     //!< TODO
};

#endif // LTTLWHWHT_KERNELTRACESYSTEM_H