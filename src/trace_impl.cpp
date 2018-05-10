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

#include "androidtoolbox.h"

// TODO
 // "IRQ Events",
 // "CPU Frequency",
 // "Memory Bus Utilization",
 // "CPU Idle",
 // "Disk I/O",
 // "eMMC commands",
 // "CPU Load",
 // "Synchronization",
 // "Kernel Workqueues",
 // "Kernel Memory Reclaim",
 // "Voltage and Current Regulators",
 // "Binder Kernel driver",
 // "Binder global lock trace",
 // "Page cache",

TraceImpl::TraceImpl(const Wire & wire, AndroidSystem * androidSystem, 
               FTrace * ftrace, FileSystem * fileSystem,
               KernelTraceSystem * kernelTraceSystem,
               FTraceBufferFile * ftraceBufferFile):
               m_Wire(wire),
               m_AndroidSystem(androidSystem),
               m_FTrace(ftrace),
               m_FileSystem(fileSystem),
               m_KernelTraceSystem(kernelTraceSystem),
               m_FTraceBufferFile(ftraceBufferFile) {
  // TODO put in arguments
  m_KernelTraceCategories["sched"]         = KernelTraceSystem::TraceCategory::SCHED;
  m_KernelTraceCategories["irq"]           = KernelTraceSystem::TraceCategory::IRQ;
  m_KernelTraceCategories["freq"]          = KernelTraceSystem::TraceCategory::FREQ;
  m_KernelTraceCategories["membus"]        = KernelTraceSystem::TraceCategory::MEMBUS;
  m_KernelTraceCategories["idle"]          = KernelTraceSystem::TraceCategory::IDLE;
  m_KernelTraceCategories["disk"]          = KernelTraceSystem::TraceCategory::DISK;
  m_KernelTraceCategories["mmc"]           = KernelTraceSystem::TraceCategory::MMC;
  m_KernelTraceCategories["load"]          = KernelTraceSystem::TraceCategory::LOAD;
  m_KernelTraceCategories["sync"]          = KernelTraceSystem::TraceCategory::SYNC;
  m_KernelTraceCategories["workqueue"]     = KernelTraceSystem::TraceCategory::WORKQUEUE;
  m_KernelTraceCategories["memreclaim"]    = KernelTraceSystem::TraceCategory::MEMRECLAIM;
  m_KernelTraceCategories["regulator"]     = KernelTraceSystem::TraceCategory::REGULATOR;
  m_KernelTraceCategories["binder_driver"] = KernelTraceSystem::TraceCategory::BINDER_DRIVER;
  m_KernelTraceCategories["binder_lock"]   = KernelTraceSystem::TraceCategory::BINDER_LOCK;
  m_KernelTraceCategories["filemap"]       = KernelTraceSystem::TraceCategory::PAGECACHE;
}

TraceImpl::~TraceImpl() {
  delete m_FileSystem;
  delete m_FTrace;
  delete m_AndroidSystem;
  delete m_KernelTraceSystem;
  delete m_FTraceBufferFile;
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
  m_Functions.insert(funcName);
}

bool TraceImpl::setUp() {
  bool ok = true;

  // Set up the tracing options.
  ok &= m_FTraceBufferFile->trySetCircularMode();
  ok &= m_FTraceBufferFile->trySetSize(m_TraceBufferSizeKB);
  ok &= setGlobalClockEnable(true);
  ok &= m_FTrace->tryEnableOption(FTrace::Option::PRINT_TGID);
  ok &= m_KernelTraceSystem->trySetFunctions(m_Functions);

  // Set up the tags property.
  printf("hello\n");
  ok &= m_AndroidSystem->tryEnableCategories(m_AndroidCategories);
  ok &= m_AndroidSystem->setAppCmdlineProperty(m_Apps);
  ok &= m_AndroidSystem->pokeBinderServices();

  // Disable all the sysfs enables.  This is done as a separate loop from
  // the enables to allow the same enable to exist in multiple categories.
  ok &= m_KernelTraceSystem->tryDisableAllCategories();

  // Enable all the sysfs enables that are in an enabled category.
  for (const auto & category : m_KernelCategories) {
    ok &= m_KernelTraceSystem->tryEnableCategory(m_KernelTraceCategories[category]);
  }
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error TraceImpl::setUp\n");
  }
  return ok;
}

void TraceImpl::cleanUp() {
  bool ok = true;
  // Disable all tracing that we're able to.
  ok &= m_KernelTraceSystem->tryDisableAllCategories();

  // Reset the system properties.
  m_AndroidSystem->disableAllCategories();
  m_AndroidSystem->clearAppProperties();
  ok &= m_AndroidSystem->pokeBinderServices();

  // Set the options back to their defaults.
  ok &= m_FTraceBufferFile->trySetCircularMode();
  ok &= m_FTraceBufferFile->trySetSize(1);
  ok &= setGlobalClockEnable(false);
  ok &= m_FTrace->tryDisableOption(FTrace::Option::PRINT_TGID);
  ok &= m_KernelTraceSystem->tryDisableAllFunctions();
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error TraceImpl::cleanUp\n");
  }
}

bool TraceImpl::setGlobalClockEnable(bool enable)
{
    const FTrace::ClockType clockType = enable ? FTrace::ClockType::GLOBAL : FTrace::ClockType::LOCAL;

    if (m_FTrace->hasTraceClockSetTo(clockType)) {
      return true;
    }
    return m_FTrace->trySetClockType(clockType);
}

bool TraceImpl::start() {
  bool ok = true;
  ok &= m_FTrace->tryStartTrace();
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error TraceImpl::start\n");
  }
  return ok;
}

void TraceImpl::stop() {
  bool ok = true;
  ok &= m_FTrace->tryStopTrace();
  if (!ok) {
    fprintf(m_Wire.getErrorStream(), "error TraceImpl::stop\n");
  }
}

bool TraceImpl::trySendTo(const string & filename) {
  int outFd = m_FileSystem->tryOpenFileToWriteOrCreate(filename.c_str());
  if (outFd == -1) {
    fprintf(m_Wire.getErrorStream(), "error DumpAction::tryRun\n");
    return false;
  }
  bool ok = m_FTraceBufferFile->trySendTo(outFd);
  close(outFd);
  return ok;
}

bool TraceImpl::trySendCompressedTo(const string & filename) {
  int outFd = m_FileSystem->tryOpenFileToWriteOrCreate(filename.c_str());
  if (outFd == -1) {
    fprintf(m_Wire.getErrorStream(), "error DumpAction::tryRun\n");
    return false;
  }
  bool ok = m_FTraceBufferFile->trySendTo(outFd);
  close(outFd);
  return ok; 
}

bool TraceImpl::trySendToOutput() {
  return m_FTraceBufferFile->trySendTo(fileno(m_Wire.getOutputStream()));
}

bool TraceImpl::trySendCompressedToOutput() {
  return m_FTraceBufferFile->trySendTo(fileno(m_Wire.getOutputStream()));
}

bool TraceImpl::tryStream(const Signal & signal) {
  return m_FTraceBufferFile->tryStreamUntil(signal);
}

bool TraceImpl::tryAddKernelCategoriesFromFile(const string & filename) {
  set<string> tokens;
  if (!AndroidToolBox().parseFileToTokens(filename.c_str(), " ", tokens)) {
    fprintf(m_Wire.getErrorStream(), "TraceImpl::tryAddKernelCategoriesFromFile - error parsing category file \"%s\"\n", filename.c_str());
    return false;
  }
  for (const auto & token : tokens) {
     addKernelCategory(token.c_str());
  }
  return true;
}

bool TraceImpl::tryEnableAndroidCoreServices() {
  set<string> tokens;
  if (m_AndroidSystem->has_core_services()) {
    string value;
    m_AndroidSystem->property_get_core_service_names(value);
    AndroidToolBox().parseToTokens(value.c_str(), ",", tokens);
    for (const auto & token : tokens) {
      addApp(token.c_str());
    }
    return true;
  }
  fprintf(m_Wire.getErrorStream(), "TraceImpl::tryEnableAndroidCoreServices - Can't enable core services - not supported\n");
  return false;
}

void TraceImpl::printSupportedCategories() {
  for (const auto & category : m_KernelTraceCategories) {
    if (m_KernelTraceSystem->supportsCategory(category.second)) {
      fprintf(m_Wire.getOutputStream(), "  %s\n", category.first.c_str());
    }
  }
  const auto & androidCategories = m_AndroidSystem->getCategories();
  for (const auto & category : androidCategories) {
      // is there a way to check?
      fprintf(m_Wire.getOutputStream(), "  %10s - %s\n", category.name, category.longname);
  }
}

bool TraceImpl::tryClear() {
  return m_FTraceBufferFile->tryClear();
}

bool TraceImpl::tryWriteClockSyncMarker() {
  return m_FTraceBufferFile->tryWriteSyncMarker();
}
