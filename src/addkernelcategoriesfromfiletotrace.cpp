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

#include "androidtoolbox.h"

bool AddKernelCategoriesFromFileToTrace::tryRunIn(Environment & environment, TraceSystem & traceSystem) {
  set<string> tokens;
  if (!AndroidToolBox().parseFileToTokens(m_Filename.c_str(), " ", tokens)) {
    fprintf(m_Wire.getErrorStream(), "AddKernelCategoriesFromFileToTrace::tryRunIn - error parsing category file \"%s\"\n", m_Filename.c_str());
    return false;
  }
  for (const auto & token : tokens) {
     traceSystem.getTrace().addKernelCategory(token.c_str());
  }
  return true;
}
