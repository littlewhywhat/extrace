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

#include "trace_impl.h"

TraceImpl::~TraceImpl() {}

void TraceImpl::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

void TraceImpl::setAndroidSystem(AndroidSystem * androidSystem) {
  m_AndroidSystem = androidSystem;
}

void TraceImpl::setKernelSystem(KernelSystem * kernelSystem) {
  m_KernelSystem = kernelSystem;
}

void TraceImpl::enableTraceOverwrite() {
  m_TraceOverwriteSwitch = true;
}

void TraceImpl::setTraceBufferSizeKB(uint32_t size) {
  m_TraceBufferSizeKB = size;
}

void TraceImpl::addKernelCategory(const char* categoryName) {
  m_KernelCategories.push_back(categoryName);
}

void TraceImpl::addAndroidCategory(const char * categoryName) {
  m_AndroidCategories.push_back(categoryName);
}

void TraceImpl::addApp(const char * appName) {
  m_Apps.push_back(appName);
}

void TraceImpl::addFunction(const char * funcName) {
  m_Functions.push_back(funcName);
}

bool TraceImpl::setUp() {
  bool ok = true;

  // Set up the tracing options.
  ok &= m_KernelSystem->setTraceOverwriteEnable(m_TraceOverwriteSwitch);
  ok &= m_KernelSystem->setTraceBufferSizeKB(m_TraceBufferSizeKB);
  ok &= m_KernelSystem->setGlobalClockEnable(true);
  ok &= m_KernelSystem->setPrintTgidEnableIfPresent(true);
  ok &= m_KernelSystem->setKernelTraceFuncs(m_Functions);

  // Set up the tags property.
  printf("hello\n");
  ok &= m_AndroidSystem->tryEnableCategories(m_AndroidCategories);
  ok &= m_AndroidSystem->setAppCmdlineProperty(m_Apps);
  ok &= m_AndroidSystem->pokeBinderServices();

  // Disable all the sysfs enables.  This is done as a separate loop from
  // the enables to allow the same enable to exist in multiple categories.
  ok &= m_KernelSystem->disableKernelTraceEvents();

  // Enable all the sysfs enables that are in an enabled category.
  ok &= m_KernelSystem->enableKernelTraceEvents(m_KernelCategories);
  if (!ok) {
    fprintf(m_ErrorStream, "error TraceImpl::setUp\n");
  }
  return ok;
}

void TraceImpl::cleanUp() {
  bool ok = true;
  // Disable all tracing that we're able to.
  ok &= m_KernelSystem->disableKernelTraceEvents();

  // Reset the system properties.
  m_AndroidSystem->disableAllCategories();
  m_AndroidSystem->clearAppProperties();
  ok &= m_AndroidSystem->pokeBinderServices();

  // Set the options back to their defaults.
  ok &= m_KernelSystem->setTraceOverwriteEnable(true);
  ok &= m_KernelSystem->setTraceBufferSizeKB(1);
  ok &= m_KernelSystem->setGlobalClockEnable(false);
  ok &= m_KernelSystem->setPrintTgidEnableIfPresent(false);
  ok &= m_KernelSystem->setKernelTraceFuncs(vector<string>());
  if (!ok) {
    fprintf(m_ErrorStream, "error TraceImpl::cleanUp\n");
  }
}

bool TraceImpl::start() {
  bool ok = true;
  ok &= m_KernelSystem->setTracingEnabled(true);
  if (!ok) {
    fprintf(m_ErrorStream, "error TraceImpl::start\n");
  }
  return ok;
}

void TraceImpl::stop() {
  bool ok = true;
  ok &= m_KernelSystem->setTracingEnabled(false);
  if (!ok) {
    fprintf(m_ErrorStream, "error TraceImpl::stop\n");
  }
}

TraceImpl * TraceImpl::Creator::createFrom(const ExtraceArguments & arguments) const
{
  auto * traceImpl = new TraceImpl();
  if (arguments.enableCircleBuffer()) {
    traceImpl->enableTraceOverwrite();
  }
  if (arguments.specifyBufferSize()) {
    traceImpl->setTraceBufferSizeKB(arguments.getBufferSize());
  }
  if (arguments.haveKernelCategories()) {
    for (auto & category : arguments.getKernelCategories()) {
      traceImpl->addKernelCategory(category.c_str());
    }
  }
  if (arguments.haveAndroidCategories()) {
    for (auto & category: arguments.getAndroidCategories()) {
      traceImpl->addAndroidCategory(category.c_str());
    }
  }
  if (arguments.haveApps()) {
    for (auto & app: arguments.getApps()) {
      traceImpl->addApp(app.c_str());
    }
  }
  if (arguments.haveKernelFunctions()) {
    for (auto & function: arguments.getKernelFunctions()) {
      traceImpl->addFunction(function.c_str());
    }
  }
  return traceImpl;
}
