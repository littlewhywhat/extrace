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
#include "android.h"

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

// TODO
// "Graphics"
// "Input"
// "View System"
// "WebView"
// "Window Manager"
// "Activity Manager"
// "Sync Manager"
// "Audio"
// "Video"
// "Camera"
// "Hardware Modules"
// "Application"
// "Resource Loading"
// "Dalvik VM"
// "RenderScript"
// "Bionic C Library"
// "Power Management"
// "Package Manager"
// "System Server"
// "Database"
// "Network"

TraceImpl::TraceImpl(const Wire & wire, AndroidTraceSystem * androidSystem, 
               FTrace * ftrace, FileSystem * fileSystem,
               KernelTraceSystem * kernelTraceSystem,
               FTraceBufferFile * ftraceBufferFile):
               m_Wire(wire),
               m_AndroidTraceSystem(androidSystem),
               m_FTrace(ftrace),
               m_FileSystem(fileSystem),
               m_KernelTraceSystem(kernelTraceSystem),
               m_FTraceBufferFile(ftraceBufferFile) {
  // TODO put in arguments

}

TraceImpl::~TraceImpl() {
  delete m_FileSystem;
  delete m_FTrace;
  delete m_AndroidTraceSystem;
  delete m_KernelTraceSystem;
  delete m_FTraceBufferFile;
}

void TraceImpl::enableTraceOverwrite() {
  m_TraceOverwriteSwitch = true;
}

void TraceImpl::setTraceBufferSizeKB(uint32_t size) {
  m_TraceBufferSizeKB = size;
}

void TraceImpl::addKernelCategory(const KernelTraceSystem::TraceCategory & category) {
  m_KernelCategories.insert(category);
}

void TraceImpl::addAndroidCategory(const Android::TraceCategory & category) {
  m_AndroidCategories.insert(category);
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
  for (const auto & category : m_AndroidCategories) {
    m_AndroidTraceSystem->rememberToTrace(category);
  }
  for (const auto & app : m_Apps) {
    m_AndroidTraceSystem->rememberToTrace(app);
  }
  ok &= m_AndroidTraceSystem->tryToTrace();

  // Disable all the sysfs enables.  This is done as a separate loop from
  // the enables to allow the same enable to exist in multiple categories.
  ok &= m_KernelTraceSystem->tryDisableAllCategories();

  // Enable all the sysfs enables that are in an enabled category.
  for (const auto & category : m_KernelCategories) {
    ok &= m_KernelTraceSystem->tryEnableCategory(category);
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
  ok &= m_AndroidTraceSystem->tryNotToTrace();

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

bool TraceImpl::tryEnableAndroidCoreServices() {
  set<string> tokens;
  if (m_AndroidTraceSystem->canTraceCoreServices()) {
    m_AndroidTraceSystem->rememberToTraceCoreServices();
    return true;
  }
  fprintf(m_Wire.getErrorStream(), "TraceImpl::tryEnableAndroidCoreServices - Can't enable core services - not supported\n");
  return false;
}

void TraceImpl::printSupportedCategories() {
  // TODO put in ListSupportedCategories
  // for (const auto & category : m_KernelTraceCategories) {
  //   if (m_KernelTraceSystem->supportsCategory(category.second)) {
  //     fprintf(m_Wire.getOutputStream(), "  %s\n", category.first.c_str());
  //   }
  // }
  // for (const auto & categoryNameAndCategory : m_AndroidTraceCategories) {
  //   fprintf(m_Wire.getOutputStream(), "  %s\n", categoryNameAndCategory.first.c_str());
  // }
}

bool TraceImpl::tryClear() {
  return m_FTraceBufferFile->tryClear();
}

bool TraceImpl::tryWriteClockSyncMarker() {
  return m_FTraceBufferFile->tryWriteSyncMarker();
}
