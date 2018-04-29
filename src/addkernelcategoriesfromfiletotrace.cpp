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

void AddKernelCategoriesFromFileToTrace::setTraceImpl(shared_ptr<TraceImpl> & traceImpl) {
  m_TraceImpl = traceImpl;
}

void AddKernelCategoriesFromFileToTrace::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

void AddKernelCategoriesFromFileToTrace::setToolbox(shared_ptr<Toolbox> & toolBox) {
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
     m_TraceImpl->addKernelCategory(token.c_str());
  }
  return true;
}

AddKernelCategoriesFromFileToTrace::Builder::Builder(FILE * errorStream,
                                        shared_ptr<TraceImpl> & traceImpl, 
                                        shared_ptr<Toolbox> & toolBox,
                                        const string & filename) {
  m_AddKernelCategoriesFromFileToTrace = new AddKernelCategoriesFromFileToTrace();
  m_AddKernelCategoriesFromFileToTrace->setErrorStream(errorStream);
  m_AddKernelCategoriesFromFileToTrace->setTraceImpl(traceImpl);
  m_AddKernelCategoriesFromFileToTrace->setToolbox(toolBox);
  m_AddKernelCategoriesFromFileToTrace->setFilename(filename);
}

AddKernelCategoriesFromFileToTrace * AddKernelCategoriesFromFileToTrace::Builder::build() const {
  return m_AddKernelCategoriesFromFileToTrace;
}