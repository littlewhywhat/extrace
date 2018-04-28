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
#include "addandroidcoretotrace.h"

void AddAndroidCoreToTrace::setTraceImpl(shared_ptr<TraceImpl> & traceImpl) {
  m_TraceImpl = traceImpl;
}

void AddAndroidCoreToTrace::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

void AddAndroidCoreToTrace::setAndroidSystem(shared_ptr<AndroidSystem> & androidSystem) {
  m_AndroidSystem = androidSystem;
}

void AddAndroidCoreToTrace::setToolbox(shared_ptr<Toolbox> & toolBox) {
  m_ToolBox = toolBox;
}

bool AddAndroidCoreToTrace::tryRun() {
  set<string> tokens;
  if (m_AndroidSystem->has_core_services()) {
    string value;
    m_AndroidSystem->property_get_core_service_names(value);
    m_ToolBox->parseToTokens(value.c_str(), ",", tokens);
    for (const auto & token : tokens) {
      m_TraceImpl->addApp(token.c_str());
    }
    return true;
  }
  fprintf(m_ErrorStream, "Can't enable core services - not supported\n");
  return false;
}

AddAndroidCoreToTrace::Builder::Builder(FILE * errorStream,
                                        shared_ptr<TraceImpl> & traceImpl, 
                                        shared_ptr<AndroidSystem> & androidSystem,
                                        shared_ptr<Toolbox> & toolBox) {
  m_AddAndroidCoreToTrace = new AddAndroidCoreToTrace();
  m_AddAndroidCoreToTrace->setErrorStream(errorStream);
  m_AddAndroidCoreToTrace->setTraceImpl(traceImpl);
  m_AddAndroidCoreToTrace->setAndroidSystem(androidSystem);
  m_AddAndroidCoreToTrace->setToolbox(toolBox);
}

AddAndroidCoreToTrace * AddAndroidCoreToTrace::Builder::build() const {
  return m_AddAndroidCoreToTrace;
}