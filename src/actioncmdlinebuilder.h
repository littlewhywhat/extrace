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

#ifndef LTTLWHWHT_ACTIONCMDLINEBUILDER_H
#define LTTLWHWHT_ACTIONCMDLINEBUILDER_H

#include "action.h"
#include "signal.h"
#include "wire.h"
#include "cmdlineargs.h"

class ActionCmdLineBuilder {
  public:
    virtual ~ActionCmdLineBuilder() {}
    virtual Action * build(const Wire & wire, Signal & signal,
                           const CmdLineArgs & cmdLineArgs) const = 0;
};

#endif // LTTLWHWHT_ACTIONCMDLINEBUILDER_H