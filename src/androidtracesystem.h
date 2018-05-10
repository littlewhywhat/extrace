/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef LTTLWHWHT_ANDROIDTRACESYSTEM_H
#define LTTLWHWHT_ANDROIDTRACESYSTEM_H

#include <string>

#include "android.h"

using namespace std;

//! I am a trace system of Android. I have Android components to trace

//! I trace all in ftrace buffer. You need to ask ftrace to trace.
//! I can trace Android trace categories. I can also trace applications 
//! and core services if it's possible.
//! TODO change my name to AndroidTraceSystem
class AndroidTraceSystem : public Wired {
  public:
    //! Constructs me Wired
    AndroidTraceSystem(const Wire & wire): Wired(wire) {}
    //! Destroys me
    virtual ~AndroidTraceSystem() {}
    //! Says if I can trace Android core services
    virtual bool canTraceCoreServices() const = 0;
    //! Says me to trace application with specified
    virtual void rememberToTrace(const string & appName) = 0;
    //! Says me to trace specified Android trace category
    virtual void rememberToTrace(const Android::TraceCategory & category) = 0;
    //! Says me to trace Android core services
    virtual void rememberToTraceCoreServices() = 0;
    //! Says me to try to trace my categories and application
    virtual bool tryToTrace() = 0;
    //! Says me to forget all applications and categories that I traced before
    virtual void forgetAll() = 0;
    //! Says me to try not to trace anymore
    virtual bool tryNotToTrace() = 0;
};

#endif // LTTLWHWHT_ANDROIDTRACESYSTEM_H