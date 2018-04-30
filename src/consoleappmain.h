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
#ifndef LTTLWHWHT_CONSOLEAPPMAIN_H
#define LTTLWHWHT_CONSOLEAPPMAIN_H

#include "consoleapp.h"

//! I am a main method wrapper to run console application
class ConsoleAppMain
{
  public:
    ConsoleAppMain(): consoleapp(NULL) {}
    ~ConsoleAppMain();
    //! Runs application
    // This method wraps main method logic and returns exit code
    int run(int argc, const char ** argv);
    //! Sets ConsoleApp to run
    // Instance of ConsoleApp that is supplied here will exist
    // until ConsoleAppMain exists.
    void set_consoleapp(ConsoleApp * consoleapp);
    void handleSignal();
  private:
    //! Pointer to console application to run
    ConsoleApp * consoleapp;
};

#endif // LTTLWHWHT_CONSOLEAPPMAIN_H