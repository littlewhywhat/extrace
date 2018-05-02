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

using namespace std;

class TraceArguments {
  public:
    TraceArguments();
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
    void setKernelCategoryFilename(const string & filename);
    void setOutputFilename(const string & filename);
    void setBufferSize(uint32_t size);
    void setInitSleepDuration(uint32_t duration);
    void setMidSleepDuration(uint32_t duration);
    void setApps(const vector<string> & apps);
    void setAndroidCategories(const vector<string> & categories);
    void setKernelCategories(const vector<string> & categories);
    void setKernelFunctions(const vector<string> & functions);
    bool hasHelpMessage() const;
    bool hasKernelCategoryFilename() const;
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
    const string & getKernelCategoryFilename() const;
    const string & getOutputFilename() const;
    const string & getHelpMessage() const;
    uint32_t getBufferSize() const;
    uint32_t getInitSleepDuration() const;
    uint32_t getMidSleepDuration() const;
    const vector<string> getApps() const;
    const vector<string> & getAndroidCategories() const;
    const vector<string> & getKernelCategories() const;
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
    string m_HelpMessage;
    string m_OutputFilename;
    string m_KernelCategoryFilename;
    uint32_t m_BufferSize = 1024;
    uint32_t m_InitSleepDuration = 0;
    uint32_t m_MidSleepDuration = 5;
    vector<string> m_Apps;
    vector<string> m_AndroidCategories;
    vector<string> m_KernelCategories;
    vector<string> m_KernelFunctions;
};

#endif // LTTLWHWHT_TRACEARGUMENTS_H