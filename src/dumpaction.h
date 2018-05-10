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

//! I am an EnvironmentAction that dumps trace buffer
class DumpAction : public EnvironmentAction {
  public:
    //! Constructs me
    DumpAction(const Wire & wire, const shared_ptr<Environment> & environment):
               EnvironmentAction(wire, environment) {}
    bool tryRun() override;
    //! Sets output file to dump trace buffer. If not set I dump to Wire output
    void setOutputFile(const string & outputFile);
    //! I am a builder of DumpAction from ExtraceArguments and Environment
    class Builder 
    {
      public:
        DumpAction * buildFrom(const Wire & wire,
                               const shared_ptr<Environment> & environment,
                               const ExtraceArguments & traceArguments) const;
    };
  private:
    //! My output file to dump in. If it's empty I dump to output
    string m_OutputFile;
    //! Tries to dump my environment trace buffer to my outputfile
    bool tryDumpToFile();
    //! Tries to dump my environment trace buffer to Wire output
    bool tryDumpToOutput();
};

#endif // LTTLWHWHT_DUMPACTION_H