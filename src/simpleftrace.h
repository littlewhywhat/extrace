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

#ifndef LTTLWHWHT_SIMPLEFTRACE_H
#define LTTLWHWHT_SIMPLEFTRACE_H

#include "wired.h"
#include "ftrace.h"

#include <map>
#include <memory>

#include "filesystem.h"
#include "toolbox.h"

using namespace std;

class SimpleFTrace : public Wired, public FTrace {
  public:
    SimpleFTrace(const Wire & wire,
           shared_ptr<FileSystem> & fileSystem,
           shared_ptr<ToolBox> toolBox,
           const string & mountPoint);
    ~SimpleFTrace() {};
    int getTraceFd() override;
    int getTracePipeFd() override;
    bool tryCleanTrace() override;
    bool tryAddFunctionToFilter(const string & function) override;
    const set<string> tryGetFunctionsFromFilter() const override;
    bool tryClearFunctionFilter() override;
    bool functionFilterAccessible() const override;
    bool trySetTracer(const FTrace::Tracer & tracer) override;
    bool trySetBufferSize(uint32_t bufferSize) override;
    bool tryWriteMarker(const char * marker) override;
    bool tracerChoiceAccessable() const override;
    bool tracePointAccessable(const FTrace::TracePoint & tracePoint) const override;
    bool tracePointExists(const FTrace::TracePoint & tracePoint) const override;
    bool tryEnableTracePoint(const FTrace::TracePoint & tracePoint) override;
    bool tryDisableTracePoint(const FTrace::TracePoint & tracePoint) override;
    bool hasTraceClockSetTo(const FTrace::ClockType & clockType) const override;
    bool trySetClockType(const FTrace::ClockType & clockType) override;
    bool tryEnableOption(const FTrace::Option & option) override;
    bool tryDisableOption(const FTrace::Option & option) override;
    bool tryStartTrace() override;
    bool tryStopTrace() override;
  private:
    string m_TraceClockPath;
    string m_TraceBufferSizePath;
    string m_CurrentTracerPath;
    string m_TraceFilterPath;
    string m_TracingOnPath;
    string m_TracePath;
    string m_TracePipePath;
    string m_TraceMarkerPath;
    map<FTrace::TracePoint, string> m_TracePoints;
    map<FTrace::Option, string> m_Options;
    map<FTrace::ClockType, string> m_ClockTypes;
    map<FTrace::Tracer, string> m_Tracers;
    shared_ptr<FileSystem> m_FileSystem = NULL;
    shared_ptr<ToolBox> m_ToolBox = NULL;
    bool tryConfigTracePoint(const TracePoint & tracePoint, bool enable);
    bool tryConfigFileSwitch(const char * filename, bool enable);
    // TODO transfer to FileSystem
    int tryOpenFile(const char * filename);
};

#endif // LTTLWHWHT_SIMPLEFTRACE_H