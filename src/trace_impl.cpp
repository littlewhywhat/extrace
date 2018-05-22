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

#include "trace_impl.h"

#include "androidtoolbox.h"
#include "android.h"

TraceImpl::TraceImpl(const Wire & wire,
                     const shared_ptr<FTrace> & ftrace,
                     const shared_ptr<AndroidTraceSystem> & androidSystem, 
                     const shared_ptr<KernelTraceSystem> & kernelTraceSystem):
               m_Wire(wire),
               m_FTrace(ftrace),
               m_AndroidTraceSystem(androidSystem),
               m_KernelTraceSystem(kernelTraceSystem) {
}

TraceImpl::~TraceImpl() {}

bool TraceImpl::setUp() {
  bool ok = true;

  // Set up the tracing options.
  if (m_CircleBufferEnabled) {
    ok &= m_FTrace->tryEnableOption(FTrace::Option::OVERWRITE);
  }
  ok &= m_FTrace->trySetBufferSize(m_TraceBufferSizeKB);
  ok &= setGlobalClockEnable(true);
  // on some machines file is not present
  m_FTrace->tryEnableOption(FTrace::Option::PRINT_TGID);
  // ok &= m_KernelTraceSystem->trySetFunctions(m_Functions);
  ok &= m_AndroidTraceSystem->tryToTrace();
  ok &= m_KernelTraceSystem->tryDisableAllCategories();
  ok &= m_KernelTraceSystem->tryToTrace();
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error TraceImpl::setUp\n");
  }
  return ok;
}

bool TraceImpl::cleanUp() {
  bool ok = true;
  ok &= m_KernelTraceSystem->tryDisableAllCategories();
  ok &= m_AndroidTraceSystem->tryNotToTrace();

  ok &= m_FTrace->tryDisableOption(FTrace::Option::OVERWRITE);
  ok &= m_FTrace->trySetBufferSize(1);
  ok &= setGlobalClockEnable(false);
  ok &= m_FTrace->tryDisableOption(FTrace::Option::PRINT_TGID);
  ok &= m_KernelTraceSystem->tryDisableAllFunctions();
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error TraceImpl::cleanUp\n");
  }
  return ok;
}

bool TraceImpl::setGlobalClockEnable(bool enable)
{
    const FTrace::ClockType clockType = enable ? FTrace::ClockType::GLOBAL : FTrace::ClockType::LOCAL;

    if (m_FTrace->hasTraceClockSetTo(clockType)) {
      return true;
    }
    return m_FTrace->trySetClockType(clockType);
}

bool TraceImpl::start() {
  bool ok = true;
  ok &= m_FTrace->tryStartTrace();
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error TraceImpl::start\n");
  }
  return ok;
}

bool TraceImpl::stop() {
  bool ok = true;
  ok &= m_FTrace->tryStopTrace();
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error TraceImpl::stop\n");
  }
  return ok;
}

void TraceImpl::setTraceBufferSizeKB(uint32_t size) {
  m_TraceBufferSizeKB = size;
}

void TraceImpl::enableCircleBuffer() {
  m_CircleBufferEnabled = true;
}