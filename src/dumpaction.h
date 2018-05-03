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

#ifndef LTTLWHWHT_DUMPACTION_H
#define LTTLWHWHT_DUMPACTION_H

#include "traceaction.h"

#include <string>
#include <memory>

#include "extracearguments.h"

class DumpAction : public TraceAction {
  public:
    DumpAction(const Wire & wire, const shared_ptr<Trace> & trace):
               TraceAction(wire, trace) {}
    bool tryRun() override;
    void setOutputFile(const string & outputFile);
    void enableCompression();
    class Builder 
    {
      public:
        TraceAction * buildFrom(const Wire & wire,
                                const shared_ptr<Trace> & trace,
                                const ExtraceArguments & traceArguments) const;
    };
  private:
    string m_OutputFile;
    bool m_Compress = false;
};

#endif // LTTLWHWHT_DUMPACTION_H