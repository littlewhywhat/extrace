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
#include "systemcore.h"

SystemCore::SystemCore() {
}

SystemCore::~SystemCore() {
  delete m_FileSystem;
  delete m_AndroidSystem;
  delete m_KernelSystem;
  delete m_Trace;
  delete m_SystemTime;
  delete m_Signal;    
}

FileSystem * SystemCore::getFileSystem() const {
  return m_FileSystem;
}

KernelSystem * SystemCore::getKernelSystem() const {
  return m_KernelSystem;
}

AndroidSystem * SystemCore::getAndroidSystem() const {
  return m_AndroidSystem;
}

Trace * SystemCore::getTrace() const {
  return m_Trace;
}

SystemTime * SystemCore::getSystemTime() const {
  return m_SystemTime;
}

Signal * SystemCore::getSignal() const {
  return m_Signal;
}

void SystemCore::setFileSystem(FileSystem * fileSystem) {
  m_FileSystem = fileSystem;
}

void SystemCore::setKernelSystem(KernelSystem * kernelSystem) {
  m_KernelSystem = kernelSystem;
}

void SystemCore::setAndroidSystem(AndroidSystem * androidSystem) {
  m_AndroidSystem = androidSystem;
}

void SystemCore::setTrace(Trace * trace) {
  m_Trace = trace;
}

void SystemCore::setSystemTime(SystemTime * systemTime) {
  m_SystemTime = systemTime;
}

void SystemCore::setSignal(Signal * signal) {
  m_Signal = signal;
}

SystemCore * SystemCore::ExtraceBuilder::build() const {
  auto * fileSystem    = new FileSystemImpl();
  auto * kernelSystem  = new KernelSystemImpl();
  auto * androidSystem = new AndroidSystemImpl();
  auto * trace         = new TraceImpl();
  auto * systemTime    = new SystemTimeImpl();
  auto * signal        = new Signal();
  auto * toolBox       = new AndroidToolbox();

  m_FileSystemImpl->set_errstream(m_ErrorStream);

  m_KernelSystemImpl->set_errstream(m_ErrorStream);
  m_KernelSystemImpl->set_file_system(sp_FileSystem);
  m_KernelSystemImpl->set_toolbox(sp_ToolBox);
  m_KernelSystemImpl->set_systime(sp_SystemTime);

  m_TraceImpl->setErrorStream(m_ErrorStream);
  m_TraceImpl->setKernelSystem(sp_KernelSystem);
  m_TraceImpl->setAndroidSystem(sp_AndroidSystem);

  SystemCore * systemCore = new SystemCore();
  systemCore->setFileSystem(fileSystem);
  systemCore->setKernelSystem(kernelSystem);
  systemCore->setAndroidSystem(androidSystem);
  systemCore->setTrace(trace);
  systemCore->setSystemTime(systemTime);
  systemCore->setSignal(signal);
}
