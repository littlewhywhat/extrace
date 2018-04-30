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

#include "androidtoolbox.h"
#include "filesystem_impl.h"
#include "androidsystem_impl.h"
#include "kernelsystem_impl.h"
#include "trace_impl.h"
#include "systemtime_impl.h"

SystemCore::SystemCore() {
}

SystemCore::~SystemCore() {
  delete m_FileSystem;
  delete m_AndroidSystem;
  delete m_KernelSystem;
  delete m_Trace;
  delete m_SystemTime;
  delete m_Signal;
  delete m_ToolBox;    
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
  FILE * errorStream       = stderr;
  FILE * outputStream      = stdout;
  auto * fileSystemImpl    = new FileSystemImpl();
  auto * kernelSystemImpl  = KernelSystemImpl::Creator().createWithDefaultCategories();
  auto * androidSystemImpl = AndroidSystemImpl::Creator().createWithDefaultCategories();
  auto * traceImpl         = new TraceImpl();
  auto * systemTimeImpl    = new SystemTimeImpl();
  auto * signal            = new Signal();
  auto * androidToolBox    = new AndroidToolBox();

  fileSystemImpl->set_errstream(errorStream);

  kernelSystemImpl->set_errstream(errorStream);
  kernelSystemImpl->set_file_system(fileSystemImpl);
  kernelSystemImpl->set_toolbox(androidToolBox);
  kernelSystemImpl->set_systime(systemTimeImpl);

  traceImpl->setErrorStream(errorStream);
  traceImpl->setKernelSystem(kernelSystemImpl);
  traceImpl->setAndroidSystem(androidSystemImpl);

  SystemCore * systemCore = new SystemCore();
  systemCore->setErrorStream(errorStream);
  systemCore->setOutputStream(outputStream);
  systemCore->setFileSystem(fileSystemImpl);
  systemCore->setKernelSystem(kernelSystemImpl);
  systemCore->setAndroidSystem(androidSystemImpl);
  systemCore->setTrace(traceImpl);
  systemCore->setSystemTime(systemTimeImpl);
  systemCore->setSignal(signal);

  return systemCore;
}
