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

void ExtraceConsoleApp::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

void ExtraceConsoleApp::register_args_to(ArgsParser & argsParser) const
{
  m_ExtraceArguments.registerTo(argsParser);
}

bool ExtraceConsoleApp::trySetup(const Arguments & arguments)
{
  m_ExtraceArguments.setArguments(&arguments);
  if (!m_ExtraceArguments.areCorrect()) {
    fprintf(m_ErrorStream, "None of necessary options specified\n");
    return false;
  }
  return true;
}

void ExtraceConsoleApp::do_run()
{
  printf("Started application\n");
}

bool ExtraceConsoleApp::do_succeeded(void) const 
{ 
  return true;
}

void ExtraceConsoleApp::printHelp(const string & appName) const {
  m_ExtraceArguments.printHelp(m_ErrorStream, appName);
}
