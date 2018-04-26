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

#include "extraceapp.h"

#include <signal.h>      // sigaction
#include <memory>
#include <string>
#include <set>

#include <utils/Trace.h> // ATRACE_TAGs

#include "argsparser.h"
#include "arguments.h"

using namespace std;

ExtraceApp::~ExtraceApp() {
  delete m_InitSleep;
  delete m_StartAction;
  delete m_MidSleep;
  delete m_StreamAction;
  delete m_StopAction;
  delete m_DumpAction;
  delete m_CleanUpAction;
}

void ExtraceApp::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

void ExtraceApp::setOutputStream(FILE * outputStream) {
  m_OutputStream = outputStream;
}

void ExtraceApp::setAndroidToolbox(AndroidToolbox * androidToolBox) {
  m_AndroidToolBox = androidToolBox;
}
void ExtraceApp::setSystemTimeImpl(SystemTimeImpl * systemTimeImpl) {
  m_SystemTimeImpl = systemTimeImpl;
}
void ExtraceApp::setFileSystemImpl(FileSystemImpl * fileSystemImpl) {
  m_FileSystemImpl = fileSystemImpl;
}
void ExtraceApp::setKernelSystemImpl(KernelSystemImpl * kernelSystemImpl) {
  m_KernelSystemImpl = kernelSystemImpl;
}
void ExtraceApp::setAndroidSystemImpl(AndroidSystemImpl * androidSystemImpl) {
  m_AndroidSystemImpl = androidSystemImpl;
}
void ExtraceApp::setTraceImpl(TraceImpl * traceImpl) {
  m_TraceImpl = traceImpl;
}
void ExtraceApp::setInitSleepAction(SleepAction * initSleep) {
  m_InitSleep = initSleep;
}
void ExtraceApp::setStartAction(StartAction * startAction) {
  m_StartAction = startAction;
}
void ExtraceApp::setMidSleepAction(SleepAction * midSleep) {
  m_MidSleep = midSleep;
}
void ExtraceApp::setStreamAction(StreamAction * streamAction) {
  m_StreamAction = streamAction;
}
void ExtraceApp::setStopAction(StopAction * stopAction) {
  m_StopAction = stopAction;
}
void ExtraceApp::setDumpAction(DumpAction * dumpAction) {
  m_DumpAction = dumpAction;
}
void ExtraceApp::setCleanUpAction(CleanUpAction * cleanUpAction) {
  m_CleanUpAction = cleanUpAction;
}
void ExtraceApp::setSignal(Signal * signal) {
  m_Signal = signal;
}

void ExtraceApp::handleSignal() {
  if (m_HandleSignals)
    m_Signal->fire();
}

void ExtraceApp::listSupportedCategories() const {
  const auto & kernelCategories = m_KernelSystemImpl->getCategories();
  for (const auto & category : kernelCategories) {
      if (m_KernelSystemImpl->isCategorySupported(category)) {
          fprintf(m_OutputStream, "  %10s - %s\n", category.name, category.longname);
      }
  }
  const auto & androidCategories = m_AndroidSystemImpl->getCategories();
  for (const auto & category : androidCategories) {
      // is there a way to check?
      fprintf(m_OutputStream, "  %10s - %s\n", category.name, category.longname);
  }
}

void ExtraceApp::showHelp(const char *cmd) const {
  fprintf(m_ErrorStream, "usage: %s [options]\n", cmd);
  fprintf(m_ErrorStream, "options include:\n"
                  "  -a appname      enable app-level tracing for a comma "
                      "separated list of cmdlines\n"
                  "  -b N            use a trace buffer size of N KB\n"
                  "  -c              trace into a circular buffer\n"
                  "  -d aname,...    trace the listed android categories\n"
                  "  -e kname,...    trace the listed kernel categories\n"
                  "  -f filename     use the categories written in a file as space-separated\n"
                  "                    values in a line\n"
                  "  -k fname,...    trace the listed kernel functions\n"
                  "  -n              ignore signals\n"
                  "  -s N            sleep for N seconds before tracing [default 0]\n"
                  "  -t N            trace for N seconds [defualt 5]\n"
                  "  -z              compress the trace dump\n"
                  "  --async_start   start circular trace and return immediatly\n"
                  "  --async_dump    dump the current contents of circular trace buffer\n"
                  "  --async_stop    stop tracing and dump the current contents of circular\n"
                  "                    trace buffer\n"
                  "  --stream        stream trace to outstream as it enters the trace buffer\n"
                  "                    Note: this can take significant CPU time, and is best\n"
                  "                    used for measuring things that are not affected by\n"
                  "                    CPU performance, like pagecache usage.\n"
                  "  --list_categories\n"
                  "                  list the available tracing categories\n"
                  " -o filename      write the trace to the specified file instead\n"
                  "                    of outstream.\n"
                  " --acore          add core services.\n"
          );  
}

void ExtraceApp::setupDependencies() {
  auto sp_ToolBox       = shared_ptr<Toolbox>(m_AndroidToolBox);
  auto sp_SystemTime    = shared_ptr<SystemTime>(m_SystemTimeImpl);
  auto sp_FileSystem    = shared_ptr<FileSystem>(m_FileSystemImpl);
  auto sp_KernelSystem  = shared_ptr<KernelSystem>(m_KernelSystemImpl);
  auto sp_AndroidSystem = shared_ptr<AndroidSystem>(m_AndroidSystemImpl);
  auto sp_Trace         = shared_ptr<Trace>(m_TraceImpl);
  auto sp_Signal        = shared_ptr<Signal>(m_Signal);
  
  m_FileSystemImpl->set_errstream(m_ErrorStream);
  m_KernelSystemImpl->set_errstream(m_ErrorStream);
  m_KernelSystemImpl->set_file_system(sp_FileSystem);
  m_KernelSystemImpl->set_toolbox(sp_ToolBox);
  m_KernelSystemImpl->set_systime(sp_SystemTime);

  m_TraceImpl->setErrorStream(m_ErrorStream);
  m_TraceImpl->setKernelSystem(sp_KernelSystem);
  m_TraceImpl->setAndroidSystem(sp_AndroidSystem);

  m_InitSleep->setSignal(sp_Signal);

  m_StartAction->setTrace(sp_Trace);
  m_StartAction->setErrorStream(m_ErrorStream);
  m_StartAction->setOutputStream(m_OutputStream);
  m_StartAction->setKernelSystem(sp_KernelSystem);

  m_MidSleep->setSignal(sp_Signal);

  m_StreamAction->setErrorStream(m_ErrorStream);
  m_StreamAction->setOutputStream(m_OutputStream);
  m_StreamAction->setKernelSystem(sp_KernelSystem);
  m_StreamAction->setSignal(sp_Signal);

  m_StopAction->setTrace(sp_Trace);
  m_StopAction->setErrorStream(m_ErrorStream);

  m_DumpAction->setErrorStream(m_ErrorStream);
  m_DumpAction->setOutputStream(m_OutputStream);
  m_DumpAction->setKernelSystem(sp_KernelSystem);

  m_CleanUpAction->setTrace(sp_Trace);
  m_CleanUpAction->setErrorStream(m_ErrorStream);
}

void ExtraceApp::setupAndroidSystemImpl() {
  m_AndroidSystemImpl->add_category( "gfx",        "Graphics",         ATRACE_TAG_GRAPHICS         );
  m_AndroidSystemImpl->add_category( "input",      "Input",            ATRACE_TAG_INPUT            );
  m_AndroidSystemImpl->add_category( "view",       "View System",      ATRACE_TAG_VIEW             );
  m_AndroidSystemImpl->add_category( "webview",    "WebView",          ATRACE_TAG_WEBVIEW          );
  m_AndroidSystemImpl->add_category( "wm",         "Window Manager",   ATRACE_TAG_WINDOW_MANAGER   );
  m_AndroidSystemImpl->add_category( "am",         "Activity Manager", ATRACE_TAG_ACTIVITY_MANAGER );
  m_AndroidSystemImpl->add_category( "sm",         "Sync Manager",     ATRACE_TAG_SYNC_MANAGER     );
  m_AndroidSystemImpl->add_category( "audio",      "Audio",            ATRACE_TAG_AUDIO            );
  m_AndroidSystemImpl->add_category( "video",      "Video",            ATRACE_TAG_VIDEO            );
  m_AndroidSystemImpl->add_category( "camera",     "Camera",           ATRACE_TAG_CAMERA           );
  m_AndroidSystemImpl->add_category( "hal",        "Hardware Modules", ATRACE_TAG_HAL              );
  m_AndroidSystemImpl->add_category( "app",        "Application",      ATRACE_TAG_APP              );
  m_AndroidSystemImpl->add_category( "res",        "Resource Loading", ATRACE_TAG_RESOURCES        );
  m_AndroidSystemImpl->add_category( "dalvik",     "Dalvik VM",        ATRACE_TAG_DALVIK           );
  m_AndroidSystemImpl->add_category( "rs",         "RenderScript",     ATRACE_TAG_RS               );
  m_AndroidSystemImpl->add_category( "bionic",     "Bionic C Library", ATRACE_TAG_BIONIC           );
  m_AndroidSystemImpl->add_category( "power",      "Power Management", ATRACE_TAG_POWER            );
  m_AndroidSystemImpl->add_category( "pm",         "Package Manager",  ATRACE_TAG_PACKAGE_MANAGER  );
  m_AndroidSystemImpl->add_category( "ss",         "System Server",    ATRACE_TAG_SYSTEM_SERVER    );
  m_AndroidSystemImpl->add_category( "database",   "Database",         ATRACE_TAG_DATABASE         );
  m_AndroidSystemImpl->add_category( "network",    "Network",          ATRACE_TAG_NETWORK          );
}

void ExtraceApp::setupKernelSystemImpl() {
  m_KernelSystemImpl->add_kernel_category("sched",         "CPU Scheduling",
  {
     { EnableFile::REQ, "/sys/kernel/debug/tracing/events/sched/sched_switch/enable" },
     { EnableFile::REQ, "/sys/kernel/debug/tracing/events/sched/sched_wakeup/enable" },
     { EnableFile::OPT, "/sys/kernel/debug/tracing/events/sched/sched_blocked_reason/enable" },
     { EnableFile::OPT, "/sys/kernel/debug/tracing/events/sched/sched_cpu_hotplug/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("irq",           "IRQ Events",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/irq/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ipi/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("freq",          "CPU Frequency",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/power/cpu_frequency/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/power/clock_set_rate/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/power/cpu_frequency_limits/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("membus",        "Memory Bus Utilization",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/memory_bus/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("idle",          "CPU Idle",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/power/cpu_idle/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("disk",          "Disk I/O",
  {
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_sync_file_enter/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_sync_file_exit/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_write_begin/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/f2fs/f2fs_write_end/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_da_write_begin/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_da_write_end/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_sync_file_enter/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ext4/ext4_sync_file_exit/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/block/block_rq_issue/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/block/block_rq_complete/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("mmc",           "eMMC commands",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/mmc/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("load",          "CPU Load",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/cpufreq_interactive/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("sync",          "Synchronization",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/sync/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("workq",         "Kernel Workqueues",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/workqueue/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("memreclaim",    "Kernel Memory Reclaim",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_begin/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_end/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_wake/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_sleep/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("regulators",    "Voltage and Current Regulators",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/regulator/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("binder_driver", "Binder Kernel driver",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction_received/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("binder_lock",   "Binder global lock trace",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_lock/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_locked/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_unlock/enable" },
  });
  m_KernelSystemImpl->add_kernel_category("pagecache",     "Page cache",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/filemap/enable" },
  }); 
}

bool ExtraceApp::addCoreServicesToTrace() {
  set<string> tokens;
  if (m_AndroidSystemImpl->has_core_services()) {
    string value;
    m_AndroidSystemImpl->property_get_core_service_names(value);
    m_AndroidToolBox->parseToTokens(value.c_str(), ",", tokens);
    for (const auto & token : tokens) {
      m_TraceImpl->addApp(token.c_str());
    }
    return true;
  }
  fprintf(m_ErrorStream, "Can't enable core services - not supported\n");
  return false;
}

bool ExtraceApp::addKernelCategoriesFromFileToTrace(const char * filename) {
  set<string> tokens;
  if (!m_AndroidToolBox->parseFileToTokens(filename, " ", tokens)) {
    fprintf(m_ErrorStream, "error parsing category file \"%s\"\n", filename);
    return false;
  }
  for (const auto & token : tokens) {
     m_TraceImpl->addKernelCategory(token.c_str());
  }
  return true;
}

bool ExtraceApp::run(int argc, const char ** argv) {
  Arguments args;
  ArgsParser argsparser;
  argsparser.setToolbox(make_shared<AndroidToolbox>());

  argsparser.register_boolean("--help", "Help");
  argsparser.register_boolean("-c", "CircleBuffer");
  argsparser.register_boolean("-n", "signalsIgnore");
  argsparser.register_boolean("-z", "Compressed");
  argsparser.register_boolean("--async_start", "AsyncStart");
  argsparser.register_boolean("--async_stop", "AsyncStop");
  argsparser.register_boolean("--async_dump", "AsyncDump");
  argsparser.register_boolean("--stream", "Stream");
  argsparser.register_boolean("--list_categories", "ListCategories");
  argsparser.register_boolean("--acore", "CoreServices");
  argsparser.register_string("-f", "KernelCategoriesFilename");
  argsparser.register_string("-o", "OutputFile");
  argsparser.register_integer("-b", "BufferSize");
  argsparser.register_integer("-s", "InitSleep");
  argsparser.register_integer("-t", "MidSleep");
  argsparser.registerCommaSepList("-a", "Apps");
  argsparser.registerCommaSepList("-d", "AndroidCategories");
  argsparser.registerCommaSepList("-e", "KernelCategories");
  argsparser.registerCommaSepList("-k", "KernelFunctions");
  
  if (argsparser.parse(args, argc, argv) != argc) {
    fprintf(m_ErrorStream, "\n");
    showHelp(argv[0]);
    return false;
  }
  if (args.is_enabled("Help")) {
    showHelp(argv[0]);
    return true;
  } 
  setupAndroidSystemImpl();
  setupKernelSystemImpl();
  setupDependencies();

  if (args.is_enabled("ListCategories")) {
    listSupportedCategories();
    return true;
  }

  if (args.is_enabled("CoreServices")) {
    if (!addCoreServicesToTrace()) {
      return false;
    }
  }
  if (args.is_enabled("CircleBuffer")) {
    m_TraceImpl->enableTraceOverwrite();
  }
  if (args.is_enabled("signalsIgnore")) {
    m_HandleSignals = false;
  }
  if (args.is_enabled("Compressed")) {
    m_DumpAction->enableCompression();
  }
  if (args.has_string("KernelCategoriesFilename")) {
     if (!addKernelCategoriesFromFileToTrace(
             args.get_string("KernelCategoriesFilename").c_str())) {
        return false;
     }
  }
  if (args.has_string("OutputFile")) {
    m_DumpAction->setOutputFile(args.get_string("OutputFile").c_str());
  }
  if (args.has_integer("BufferSize")) {
    m_TraceImpl->setTraceBufferSizeKB(args.get_integer("BufferSize"));
  }
  if (args.has_integer("InitSleep")) {
    m_InitSleep->setDurationSeconds(args.get_integer("InitSleep"));
  }
  if (args.has_integer("MidSleep")) {
    m_MidSleep->setDurationSeconds(args.get_integer("MidSleep"));
  }
  if (args.hasStringList("Apps")) {
    for (const auto & appname : args.getStringList("Apps")) {
      m_TraceImpl->addApp(appname.c_str());
    }
  }
  if (args.hasStringList("AndroidCategories")) {
    for (const auto & androidCategory : args.getStringList("AndroidCategories")) {
      m_TraceImpl->addAndroidCategory(androidCategory.c_str());
    }
  }
  if (args.hasStringList("KernelCategories")) {
    for (const auto & kernelCategory : args.getStringList("KernelCategories")) {
      m_TraceImpl->addKernelCategory(kernelCategory.c_str());
    }
  }
  if (args.hasStringList("KernelFunctions")) {
    for (const auto & kernelFunction : args.getStringList("KernelFunctions")) {
      m_TraceImpl->addFunction(kernelFunction.c_str());
    }
  }

  ActionRunnerImpl actionRunnerImpl;
  if (args.has_integer("InitSleep")) {
    actionRunnerImpl.addAction(m_InitSleep);
  }
  if (args.is_enabled("AsyncStart")) {
    actionRunnerImpl.addAction(m_StartAction);
    if (args.is_enabled("Stream")) {
      actionRunnerImpl.addAction(m_StreamAction);
      m_HandleSignals = true;
    }
  }
  else if (args.is_enabled("AsyncStop")) {
    actionRunnerImpl.addAction(m_StopAction);
    actionRunnerImpl.addAction(m_DumpAction);
    actionRunnerImpl.addAction(m_CleanUpAction);
  }
  else if (args.is_enabled("AsyncDump")) {
    actionRunnerImpl.addAction(m_DumpAction);
  }
  else {
    actionRunnerImpl.addAction(m_StartAction);
    actionRunnerImpl.addAction(m_MidSleep);
    if (args.is_enabled("Stream")) {
      actionRunnerImpl.addAction(m_StreamAction);
      m_HandleSignals = true;
    }
    actionRunnerImpl.addAction(m_StopAction);
    actionRunnerImpl.addAction(m_DumpAction);
    actionRunnerImpl.addAction(m_CleanUpAction);
  }

  return actionRunnerImpl.tryRunActions();
}
