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

#ifndef LTTLWHWHT_STREAMACTION_H
#define LTTLWHWHT_STREAMACTION_H

#include "action.h"

#include <memory>   // shared_ptr
#include <stdio.h>  // FILE

#include "kernelsystem.h"
#include "signal.h"

using namespace std;

class StreamAction : public Action {
  public:
    bool tryRun() override;
    void setSignal(shared_ptr<Signal> & signal);
    void setErrorStream(FILE * errorStream);
    void setOutputStream(FILE * outStream);
    void setKernelSystem(shared_ptr<KernelSystem> & kernelSystem);
  private:
    shared_ptr<KernelSystem> m_KernelSystem;
    shared_ptr<Signal> m_Signal;
    FILE * m_ErrorStream;
    FILE * m_OutStream;
};

#endif // LTTLWHWHT_STREAMACTION_H