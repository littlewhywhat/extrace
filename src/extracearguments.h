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

#ifndef LTTLWHWHT_TRACEARGUMENTS_H
#define LTTLWHWHT_TRACEARGUMENTS_H

#include <string>
#include <vector>
#include <stdint.h>

#include "android.h"
#include "kerneltracesystem.h"

using namespace std;

class ExtraceArguments {
  public:
    ExtraceArguments();
    void setPIDs(const vector<int> & pids);
    const vector<int> & getPIDs() const;
    bool hasPIDs() const;
    void setPeriod(uint32_t period);
    uint32_t getPeriod() const;
    bool hasPeriod() const;
    void setTimes(uint32_t period);
    uint32_t getTimes() const;
    bool hasTimes() const;
    void setAppName(const string & appName);
    void setHelpMessage(const string & message);
    void enableCircleBuffer();
    void enableIgnoreSignals();
    void enableCompression();
    void enableAsyncStart();
    void enableAsyncStop();
    void enableAsyncDump();
    void enableStream();
    void enableListCategories();
    void enableCoreServices();
    void setOutputFilename(const string & filename);
    void setBufferSize(uint32_t size);
    void setInitSleepDuration(uint32_t duration);
    void setMidSleepDuration(uint32_t duration);
    void setApps(const vector<string> & apps);
    void addAndroidCategory(const Android::TraceCategory & traceCategory);
    void addKernelCategory(const KernelTraceSystem::TraceCategory & traceCategory);
    void setKernelFunctions(const vector<string> & functions);
    bool hasHelpMessage() const;
    bool hasOutputFilename() const;
    bool hasApps() const;
    bool hasAndroidCategories() const;
    bool hasKernelCategories() const;
    bool hasKernelFunctions() const;
    bool hasInitSleepDuration() const;
    bool hasMidSleepDuration() const;
    bool circleBufferEnabled() const;
    bool ignoreSignalsEnabled() const;
    bool compressionEnabled() const;
    bool asyncStartEnabled() const;
    bool asyncStopEnabled() const;
    bool asyncDumpEnabled() const;
    bool streamEnabled() const;
    bool listCategoriesEnabled() const;
    bool coreServicesEnabled() const;
    const string & getAppName() const;
    const string & getOutputFilename() const;
    const string & getHelpMessage() const;
    uint32_t getBufferSize() const;
    uint32_t getInitSleepDuration() const;
    uint32_t getMidSleepDuration() const;
    const vector<string> & getApps() const;
    const vector<Android::TraceCategory> & getAndroidCategories() const;
    const vector<KernelTraceSystem::TraceCategory> & getKernelCategories() const;
    const vector<string> & getKernelFunctions() const;
  private:
    bool m_CircleBuffer = false;
    bool m_IgnoreSignals = false;
    bool m_Compression = false;
    bool m_AsyncStart = false;
    bool m_AsyncStop = false;
    bool m_AsyncDump = false;
    bool m_Stream = false;
    bool m_ListCategories = false;
    bool m_CoreServices = false;
    string m_AppName;
    string m_HelpMessage;
    string m_OutputFilename;
    string m_KernelCategoryFilename;
    uint32_t m_BufferSize = 1024;
    uint32_t m_InitSleepDuration = 0;
    uint32_t m_MidSleepDuration = 5;
    uint32_t m_Period = 0;
    uint32_t m_Times = 0;
    vector<int> m_PIDs;
    vector<string> m_Apps;
    vector<Android::TraceCategory> m_AndroidCategories;
    vector<KernelTraceSystem::TraceCategory> m_KernelCategories;
    vector<string> m_KernelFunctions;
};

#endif // LTTLWHWHT_TRACEARGUMENTS_H