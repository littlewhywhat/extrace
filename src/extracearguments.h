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

#ifndef LTTLWHWHT_EXTRACEARGUMENTS_H
#define LTTLWHWHT_EXTRACEARGUMENTS_H

#include "arguments.h"
#include "argsparser.h"

class ExtraceArguments 
{
  public:
    void setArguments(const Arguments * arguments);
    void registerTo(ArgsParser & argsParser) const;
    bool areCorrect() const;
    void printHelp(FILE * out, const string & appName) const;
    bool haveHelpOption() const;
    bool haveListCategoriesOption() const;
    bool haveDumpFilename() const;
    const string & getDumpFilename() const;
    bool enableCompression() const;
    bool enableAsyncStart() const;
    bool enableAsyncStop() const;
    bool enableAsyncDump() const;
    bool enableStream() const;
    bool enableListCategories() const;
    bool enableCoreServices() const;
    bool enableCircleBuffer() const;
    bool enableSignalsIgnore() const;
    bool haveKernelCategoryFilename() const;
    const string & getKernelCategoryFilename() const;
    bool specifyBufferSize() const;
    int getBufferSize() const;
    bool specifyInitSleepDuration() const;
    int getInitSleepDuration() const;
    bool specifyMidSleepDuration() const;
    int getMidSleepDuration() const;
    bool haveApps() const;
    const vector<string> & getApps() const;
    bool haveAndroidCategories() const;
    const vector<string> & getAndroidCategories() const;
    bool haveKernelCategories() const;
    const vector<string> & getKernelCategories() const;
    bool haveKernelFunctions() const;
    const vector<string> & getKernelFunctions() const;
  private:
    const Arguments * m_Arguments;
};

#endif // LTTLWHWHT_EXTRACEARGUMENTS_H