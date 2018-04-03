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

#ifndef LTTLWHWHT_CONSOLEAPP_H
#define LTTLWHWHT_CONSOLEAPP_H

//! I am a console application to run using console arguments
class ConsoleApp
{
  public:
    virtual ~ConsoleApp() {}
    //! Setups ConsoleApp by console args to run
    virtual void setup(int argc, const char ** argv) = 0;
    //! Indicates if app is setup and is ready to run
    virtual bool is_ready(void) const = 0;
    //! Runs app
    virtual void run() = 0;
    //! Indicates if run was successful
    virtual bool succeeded(void) const = 0;
};

#endif // LTTLWHWHT_CONSOLEAPP_H