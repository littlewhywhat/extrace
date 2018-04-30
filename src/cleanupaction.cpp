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
#include "cleanupaction.h"

#include <cstdio>

void CleanUpAction::setTrace(Trace * trace) {
  m_Trace = trace;
}

void CleanUpAction::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

bool CleanUpAction::tryRun() {
  bool ok = true;
  m_Trace->cleanUp();
  if (!ok) {
    fprintf(m_ErrorStream, "error CleanUpAction::tryRun\n");
  }
  return ok;
}

Action * CleanUpAction::Builder::buildFrom(const SystemCore & systemCore)
{
  auto * cleanUpAction = new CleanUpAction();
  cleanUpAction->setTrace(systemCore.getTrace());
  cleanUpAction->setErrorStream(systemCore.getErrorStream());
  return cleanUpAction;
}