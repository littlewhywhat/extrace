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
#include <cstdlib> // EXIT_SUCCESS, EXIT_FAILURE

#include "consoleappmain.h"

int ConsoleAppMain::run(int argc, const char ** argv)
{
  ConsoleApp & consoleapp = *(this->consoleapp);
  consoleapp.setName(argv[0]);
  consoleapp.setup(argc, argv);
  if (consoleapp.is_ready())
  {
    consoleapp.run();
    if (consoleapp.succeeded())
    {
      return EXIT_SUCCESS;
    }
  }
  return EXIT_FAILURE;
}

ConsoleAppMain::~ConsoleAppMain()
{
  delete this->consoleapp;
}

void ConsoleAppMain::set_consoleapp(ConsoleApp * consoleapp)
{
  this->consoleapp = consoleapp;
}

void ConsoleAppMain::handleSignal()
{
  this->consoleapp->handleSignal();
}