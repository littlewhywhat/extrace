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

#ifndef LTTLWHWHT_TRACEARGUMENTSBUILDER_H
#define LTTLWHWHT_TRACEARGUMENTSBUILDER_H

#include "arguments.h"
#include "tracearguments.h"
#include "wire.h"
#include "cmdlineargs.h"
#include "cmdlineargsparser.h"

#include <string>

class TraceArgumentsBuilder {
  public:
    const TraceArguments * build(const Wire & wire, const CmdLineArgs & cmdLineArgs) const;
  private:
    void registerCmdLineOpts(CmdLineArgsParser & cmdLineArgsParser) const;
    TraceArguments * createHelpTraceArguments() const;
    TraceArguments * createTraceArguments(const Arguments & arguments) const;
    const string & getHelpMessage() const;
};

#endif // LTTLWHWHT_TRACEARGUMENTSBUILDER_H