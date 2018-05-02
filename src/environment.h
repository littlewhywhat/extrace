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

#ifndef LTTLWHWHT_ENVIRONMENT_H
#define LTTLWHWHT_ENVIRONMENT_H

#include <string>

using namespace std;

class Environment {
  public:
    Environment(const string & appName): m_AppName(appName) {}
    const string & getAppName() const;
  private:
    const string & m_AppName;
};

#endif // LTTLWHWHT_ENVIRONMENT_H