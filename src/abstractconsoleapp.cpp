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
#include "abstractconsoleapp.h"

#include "arguments.h"
#include "argsparser.h"
#include "androidtoolbox.h"

void AbstractConsoleApp::setup(int argc, const char ** argv) {
  // TODO refactor
  // ArgsParser consoleArgsParser(argc, argv);
  ArgsParser consoleArgsParser;
  consoleArgsParser.setToolbox(make_shared<AndroidToolbox>());
  this->register_args_to(consoleArgsParser);
  Arguments arguments;
  // TODO refactor
  // consoleArgsParser.parse_to(arguments);
  // if (consoleArgsParser.succeeded()
  if (consoleArgsParser.parse(arguments, argc, argv) == argc
      && this->trySetup(arguments))
  {
    this->isReady = true;
  }
  else
  {
    this->isReady = false;
    this->printHelp(argv[0]); 
  }
}

bool AbstractConsoleApp::is_ready(void) const { return this->isReady; }
