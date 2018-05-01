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

#include "tracesystem.h"

TraceSystem::~TraceSystem() {
  delete m_FileSystem;
  delete m_KernelSystem;
  delete m_AndroidSystem;
  delete m_Trace;
}

FileSystem & TraceSystem::getFileSystem() const {
  return *m_FileSystem;
}

KernelSystem & TraceSystem::getKernelSystem() const {
  return *m_KernelSystem;
}

AndroidSystem & TraceSystem::getAndroidSystem() const {
  return *m_AndroidSystem;
}

Trace & TraceSystem::getTrace() const {
  return *m_Trace;
}
