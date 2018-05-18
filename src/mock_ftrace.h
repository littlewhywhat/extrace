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

#ifndef LTTLWHWHT_MOCK_FTRACE_H
#define LTTLWHWHT_MOCK_FTRACE_H

#include <gmock/gmock.h>

#include "ftrace.h"

class MockFTrace : public FTrace {
  public:
    MOCK_METHOD0(getTraceFd, int());
    MOCK_METHOD0(getTracePipeFd, int());
    MOCK_METHOD0(tryCleanTrace, bool());
    MOCK_METHOD1(tryAddFunctionToFilter, bool(const string & function));
    MOCK_CONST_METHOD0(tryGetFunctionsFromFilter, const set<string>());
    MOCK_METHOD0(tryClearFunctionFilter, bool());
    MOCK_CONST_METHOD0(functionFilterAccessible, bool());
    MOCK_METHOD1(trySetTracer, bool(const Tracer & tracer));
    MOCK_METHOD1(trySetBufferSize, bool(uint32_t bufferSize));
    MOCK_METHOD1(tryWriteMarker, bool(const char * marker));
    MOCK_CONST_METHOD0(tracerChoiceAccessable, bool());
    MOCK_CONST_METHOD1(tracePointAccessable, bool(const TracePoint & tracePoint));
    MOCK_CONST_METHOD1(tracePointExists, bool(const TracePoint & tracePoint));
    MOCK_METHOD1(tryEnableTracePoint, bool(const TracePoint & tracePoint));
    MOCK_METHOD1(tryDisableTracePoint, bool(const TracePoint & tracePoint));
    MOCK_CONST_METHOD1(hasTraceClockSetTo, bool(const ClockType & clockType));
    MOCK_METHOD1(trySetClockType, bool(const ClockType & clockType));
    MOCK_METHOD1(tryEnableOption, bool(const Option & option));
    MOCK_METHOD1(tryDisableOption, bool(const Option & option));
    MOCK_METHOD0(tryStartTrace, bool());
    MOCK_METHOD0(tryStopTrace, bool());
};

#endif // LTTLWHWHT_MOCK_FTRACE_H