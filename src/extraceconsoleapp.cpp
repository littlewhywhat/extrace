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
#include "actionrunnerbuilder.h"

#include <stdio.h>

ExtraceConsoleApp::~ExtraceConsoleApp() {
  delete m_ActionRunner;
  delete m_ActionRunnerBuilder;
}

void ExtraceConsoleApp::setActionRunnerBuilder(ActionRunnerBuilder * builder)
{
  m_ActionRunnerBuilder = builder;
}

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
  if (m_ExtraceArguments.haveHelpOption()) {
    return false;
  }
  m_ActionRunner = m_ActionRunnerBuilder->buildFrom(m_ExtraceArguments);
  return true;
}

void ExtraceConsoleApp::do_run()
{
  printf("Started application\n");
  m_Success = m_ActionRunner->tryRunActions();
}

bool ExtraceConsoleApp::do_succeeded(void) const 
{ 
  return m_Success;
}

void ExtraceConsoleApp::printHelp() const {
  m_ExtraceArguments.printHelp(m_ErrorStream, m_Name);
}
