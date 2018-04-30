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
#include "addkernelcategoriesfromfiletotrace.h"

void AddKernelCategoriesFromFileToTrace::setTrace(Trace * trace) {
  m_Trace = trace;
}

void AddKernelCategoriesFromFileToTrace::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

void AddKernelCategoriesFromFileToTrace::setToolBox(ToolBox * toolBox) {
  m_ToolBox = toolBox;
}

void AddKernelCategoriesFromFileToTrace::setFilename(const string & filename) {
  m_Filename = filename;
}

bool AddKernelCategoriesFromFileToTrace::tryRun() {
  set<string> tokens;
  if (!m_ToolBox->parseFileToTokens(m_Filename.c_str(), " ", tokens)) {
    fprintf(m_ErrorStream, "error parsing category file \"%s\"\n", m_Filename.c_str());
    return false;
  }
  for (const auto & token : tokens) {
     m_Trace->addKernelCategory(token.c_str());
  }
  return true;
}

Action * AddKernelCategoriesFromFileToTrace::Builder::buildFrom(const SystemCore & systemCore,
                                                            const ExtraceArguments & arguments) const {
  auto * addKernelCategoriesFromFileToTrace = new AddKernelCategoriesFromFileToTrace();
  if (arguments.haveKernelCategoryFilename())
  {
    addKernelCategoriesFromFileToTrace->setFilename(arguments.getKernelCategoryFilename());
  }
  addKernelCategoriesFromFileToTrace->setErrorStream(systemCore.getErrorStream());
  addKernelCategoriesFromFileToTrace->setTrace(systemCore.getTrace());
  addKernelCategoriesFromFileToTrace->setToolBox(systemCore.getToolBox());
  return addKernelCategoriesFromFileToTrace;
}