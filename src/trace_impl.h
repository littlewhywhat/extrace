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

#ifndef LTTLWHWHT_TRACE_IMPL_H
#define LTTLWHWHT_TRACE_IMPL_H

#include "trace.h"

#include "wire.h"
#include "simpleftrace.h"
#include "androidtracesystem.h"
#include "kerneltracesystem.h"

#include <string>
#include <memory>
#include <cstdio>
#include <unistd.h> // close

using namespace std;

class TraceImpl : public Trace {
  public:
    TraceImpl(const Wire & wire,
              const shared_ptr<FTrace> & ftrace,
              const shared_ptr<AndroidTraceSystem> & androidSystem, 
              const shared_ptr<KernelTraceSystem> & kernelTraceSystem);
    ~TraceImpl();
    // Set all the kernel tracing settings to the desired state for this trace
    // capture.
    bool setUp() override;
    // Reset all the kernel tracing settings to their default state.
    bool cleanUp() override;
    // Enable tracing in the kernel.
    bool start() override;
    // Disable tracing in the kernel.
    bool stop() override;
    void enableCircleBuffer();
    void setTraceBufferSizeKB(uint32_t size);
  private:
    const Wire & m_Wire;
    shared_ptr<FTrace> m_FTrace;
    shared_ptr<AndroidTraceSystem> m_AndroidTraceSystem;
    shared_ptr<KernelTraceSystem> m_KernelTraceSystem;
    uint32_t m_TraceBufferSizeKB = 1024;
    bool m_CircleBufferEnabled = false;
    bool setGlobalClockEnable(bool enable);
};

#endif // LTTLWHWHT_TRACE_IMPL_H