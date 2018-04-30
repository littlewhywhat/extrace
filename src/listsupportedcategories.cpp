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
#include "listsupportedcategories.h"

void ListSupportedCategories::setAndroidSystem(AndroidSystem * androidSystem) {
  m_AndroidSystem = androidSystem;
}

void ListSupportedCategories::setKernelSystem(KernelSystem * kernelSystem) {
  m_KernelSystem = kernelSystem;
}

void ListSupportedCategories::setOutputStream(FILE * outputStream) {
  m_OutputStream = outputStream;
}

bool ListSupportedCategories::tryRun() {
  const auto & kernelCategories = m_KernelSystem->getCategories();
  for (const auto & category : kernelCategories) {
      if (m_KernelSystem->isCategorySupported(category)) {
          fprintf(m_OutputStream, "  %10s - %s\n", category.name, category.longname);
      }
  }
  const auto & androidCategories = m_AndroidSystem->getCategories();
  for (const auto & category : androidCategories) {
      // is there a way to check?
      fprintf(m_OutputStream, "  %10s - %s\n", category.name, category.longname);
  }
  return true;
}

Action * ListSupportedCategories::Builder::buildFrom(const SystemCore & systemCore) const {
  auto * listSupportedCategories = new ListSupportedCategories();
  listSupportedCategories->setOutputStream(systemCore.getOutputStream());
  listSupportedCategories->setAndroidSystem(systemCore.getAndroidSystem());
  listSupportedCategories->setKernelSystem(systemCore.getKernelSystem());
  return listSupportedCategories;
}
