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

#ifndef LTTLWHWHT_LISTSUPPORTEDCATEGORIES_H
#define LTTLWHWHT_LISTSUPPORTEDCATEGORIES_H

#include "action.h"

#include <memory>
#include <cstdio>

#include "androidsystem.h"
#include "kernelsystem.h"

using namespace std;

class ListSupportedCategories : public Action {
  public:
    bool tryRun() override;
    void setOutputStream(FILE * outputStream);
    void setAndroidSystem(shared_ptr<AndroidSystem> & AndroidSystem);
    void setKernelSystem(shared_ptr<KernelSystem> & kernelSystem);
    class Builder {
      public:
        Builder(FILE * outputStream,
                shared_ptr<AndroidSystem> & androidSystem, 
                shared_ptr<KernelSystem> & kernelSystem);
        ListSupportedCategories * build() const;
      private:
        ListSupportedCategories * m_ListSupportedCategories;
    };
  private:
    shared_ptr<AndroidSystem> m_AndroidSystem;
    shared_ptr<KernelSystem> m_KernelSystem;
    FILE * m_OutputStream;
};

#endif // LTTLWHWHT_LISTSUPPORTEDCATEGORIES_H