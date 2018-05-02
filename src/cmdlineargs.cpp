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

#include "cmdlineargs.h"

CmdLineArgs::CmdLineArgs(int argc, const char** argv) {
  m_AppName = argv[0];
  for (int i = 1; i < argc; i++) {
    m_Args.push_back(argv[i]);
  }
}

uint32_t CmdLineArgs::getCount() const {
  return m_Args.size();
}

const string & CmdLineArgs::get(uint32_t id) const {
  return m_Args[id];
}

const string & CmdLineArgs::getAppName() const {
  return m_AppName;
}
