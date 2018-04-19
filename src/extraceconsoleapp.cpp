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
#include "extraceconsoleapp.h"

#include <stdio.h>

static const char * CATEGORY_SINGLE_OPTION_NAME = "category";

void ExtraceConsoleApp::register_args_to(ArgsParser & argsParser) const
{
  argsParser.register_string("-c", CATEGORY_SINGLE_OPTION_NAME);
}

bool ExtraceConsoleApp::admits(const Arguments & arguments) const
{
  return arguments.has_string(CATEGORY_SINGLE_OPTION_NAME);
}

void ExtraceConsoleApp::setup_with(const Arguments & arguments)
{
  this->category = arguments.get_string(CATEGORY_SINGLE_OPTION_NAME);
}

void ExtraceConsoleApp::do_run()
{
  printf("Selected category is %s.\n", this->category.c_str());
}

bool ExtraceConsoleApp::do_succeeded(void) const 
{ 
  return true;
}
