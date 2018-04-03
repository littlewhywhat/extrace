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
#ifndef LTTLWHWHT_ABSTRCONSOLEAPP_H
#define LTTLWHWHT_ABSTRCONSOLEAPP_H

#include "argsparser.h"
#include "arguments.h"
#include "consoleapp.h"

//! I contain common logic of ConsoleApp implementations
class AbstractConsoleApp : public ConsoleApp
{
  public:
    AbstractConsoleApp(): isReady(false) {}
    virtual ~AbstractConsoleApp() {}
    void setup(int argc, const char ** argv) override;
    bool is_ready(void) const override;
    void run() override { do_run(); }
    bool succeeded(void) const override { return do_succeeded(); }
  private:
    //! Stub for limiting inheritors 
    virtual void do_run() = 0;
    //! Stub for limiting inheritors
    virtual bool do_succeeded(void) const = 0;
    //! Registers arguments to parse from provided args in setup
    // Inheritors should register to argsParser arguments that 
    // they want to try to get from argv
    virtual void register_args_to(ArgsParser & argsParser) const = 0;
    //! Validates parsed arguments
    virtual bool admits(const Arguments & arguments) const = 0;
    //! Setups itself to run using parsed arguments
    virtual void setup_with(const Arguments & arguments) = 0;
    //! Boolean flag to indicate if app is ready to run
    bool isReady;
};

#endif // LTTLWHWHT_ABSTRCONSOLEAPP_H