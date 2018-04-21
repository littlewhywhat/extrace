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

#define LOG_TAG "atrace"

#include "filesystem_impl.h"
#include "systemtime_impl.h"
#include "androidsystem_impl.h"
#include "kernelsystem_impl.h"
#include "androidtoolbox.h"
#include "trace_impl.h"
#include "signal.h"
#include "startaction.h"
#include "stopaction.h"
#include "sleepaction.h"
#include "dumpaction.h"
#include "streamaction.h"
#include "cleanupaction.h"
#include "filesystem.h"
#include "systemtime.h"
#include "androidsystem.h"
#include "kernelsystem.h"
#include "toolbox.h"
#include "trace.h"
#include "action.h"
#include "actionrunner_impl.h"

#include <getopt.h>      // getopt_long, no_argument
#include <signal.h>      // sigaction
#include <utils/Trace.h> // ATRACE_TAGs
#include <stdlib.h>      // EXIT_FAILURE, EXIT_SUCCESS
#include <memory>

using namespace std;

bool handleSignals = true;

auto * errorStream    = stderr;
auto * outputStream   = stdout;
auto toolBox        = new AndroidToolbox();
auto systemTimeImpl = new SystemTimeImpl();
auto fileSystem     = new FileSystemImpl();
auto kernelSystem   = new KernelSystemImpl();
auto androidSystem  = new AndroidSystemImpl();
auto trace          = new TraceImpl();
auto initSleep      = new SleepAction();
auto startAction    = new StartAction();
auto midSleep       = new SleepAction();
auto streamAction   = new StreamAction();
auto stopAction     = new StopAction();
auto dumpAction     = new DumpAction();
auto cleanUpAction  = new CleanUpAction();
auto signal_impl    = new Signal();

void handleSignal(int /*signo*/)
{
  signal_impl->fire();
}

void registerSigHandler()
{
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handleSignal;
  sigaction(SIGHUP, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGQUIT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);
}

void listSupportedCategories() {
  const auto & kernelCategories = kernelSystem->getCategories();
  for (const auto & category : kernelCategories) {
      if (kernelSystem->isCategorySupported(category)) {
          printf("  %10s - %s\n", category.name, category.longname);
      }
  }
  const auto & androidCategories = androidSystem->getCategories();
  for (const auto & category : androidCategories) {
      // is there a way to check?
      printf("  %10s - %s\n", category.name, category.longname);
  }
}

// Print the command usage help to errorStream.
void showHelp(const char *cmd)
{
    fprintf(errorStream, "usage: %s [options] [categories...]\n", cmd);
    fprintf(errorStream, "options include:\n"
                    "  -a appname      enable app-level tracing for a comma "
                        "separated list of cmdlines\n"
                    "  -b N            use a trace buffer size of N KB\n"
                    "  -c              trace into a circular buffer\n"
                    "  -d aname,...    trace the listed android categories\n"
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
            );
}

void setupDependencies() {
  auto sp_ToolBox       = shared_ptr<Toolbox>(toolBox);
  auto sp_SystemTime    = shared_ptr<SystemTime>(systemTimeImpl);
  auto sp_FileSystem    = shared_ptr<FileSystem>(fileSystem);
  auto sp_KernelSystem  = shared_ptr<KernelSystem>(kernelSystem);
  auto sp_AndroidSystem = shared_ptr<AndroidSystem>(androidSystem);
  auto sp_Trace         = shared_ptr<Trace>(trace);
  auto sp_Signal        = shared_ptr<Signal>(signal_impl);
  
  fileSystem->set_errstream(errorStream);
  kernelSystem->set_errstream(errorStream);
  kernelSystem->set_file_system(sp_FileSystem);
  kernelSystem->set_toolbox(sp_ToolBox);
  kernelSystem->set_systime(sp_SystemTime);
  androidSystem->set_errstream(errorStream);

  trace->setErrorStream(errorStream);
  trace->setKernelSystem(sp_KernelSystem);
  trace->setAndroidSystem(sp_AndroidSystem);

  initSleep->setSignal(sp_Signal);

  startAction->setTrace(sp_Trace);
  startAction->setErrorStream(errorStream);
  startAction->setOutputStream(outputStream);
  startAction->setKernelSystem(sp_KernelSystem);

  midSleep->setSignal(sp_Signal);

  streamAction->setErrorStream(errorStream);
  streamAction->setOutputStream(outputStream);
  streamAction->setKernelSystem(sp_KernelSystem);
  streamAction->setSignal(sp_Signal);

  stopAction->setTrace(sp_Trace);
  stopAction->setErrorStream(errorStream);

  dumpAction->setErrorStream(errorStream);
  dumpAction->setOutputStream(outputStream);
  dumpAction->setKernelSystem(sp_KernelSystem);

  cleanUpAction->setTrace(sp_Trace);
  cleanUpAction->setErrorStream(errorStream);
}

void setupAndroidSystemImpl() {
  androidSystem->add_category( "gfx",        "Graphics",         ATRACE_TAG_GRAPHICS         );
  androidSystem->add_category( "input",      "Input",            ATRACE_TAG_INPUT            );
  androidSystem->add_category( "view",       "View System",      ATRACE_TAG_VIEW             );
  androidSystem->add_category( "webview",    "WebView",          ATRACE_TAG_WEBVIEW          );
  androidSystem->add_category( "wm",         "Window Manager",   ATRACE_TAG_WINDOW_MANAGER   );
  androidSystem->add_category( "am",         "Activity Manager", ATRACE_TAG_ACTIVITY_MANAGER );
  androidSystem->add_category( "sm",         "Sync Manager",     ATRACE_TAG_SYNC_MANAGER     );
  androidSystem->add_category( "audio",      "Audio",            ATRACE_TAG_AUDIO            );
  androidSystem->add_category( "video",      "Video",            ATRACE_TAG_VIDEO            );
  androidSystem->add_category( "camera",     "Camera",           ATRACE_TAG_CAMERA           );
  androidSystem->add_category( "hal",        "Hardware Modules", ATRACE_TAG_HAL              );
  androidSystem->add_category( "app",        "Application",      ATRACE_TAG_APP              );
  androidSystem->add_category( "res",        "Resource Loading", ATRACE_TAG_RESOURCES        );
  androidSystem->add_category( "dalvik",     "Dalvik VM",        ATRACE_TAG_DALVIK           );
  androidSystem->add_category( "rs",         "RenderScript",     ATRACE_TAG_RS               );
  androidSystem->add_category( "bionic",     "Bionic C Library", ATRACE_TAG_BIONIC           );
  androidSystem->add_category( "power",      "Power Management", ATRACE_TAG_POWER            );
  androidSystem->add_category( "pm",         "Package Manager",  ATRACE_TAG_PACKAGE_MANAGER  );
  androidSystem->add_category( "ss",         "System Server",    ATRACE_TAG_SYSTEM_SERVER    );
  androidSystem->add_category( "database",   "Database",         ATRACE_TAG_DATABASE         );
  androidSystem->add_category( "network",    "Network",          ATRACE_TAG_NETWORK          );
}

void setupKernelSystemImpl() {
  kernelSystem->add_kernel_category("sched",         "CPU Scheduling",
  {
     { EnableFile::REQ, "/sys/kernel/debug/tracing/events/sched/sched_switch/enable" },
     { EnableFile::REQ, "/sys/kernel/debug/tracing/events/sched/sched_wakeup/enable" },
     { EnableFile::OPT, "/sys/kernel/debug/tracing/events/sched/sched_blocked_reason/enable" },
     { EnableFile::OPT, "/sys/kernel/debug/tracing/events/sched/sched_cpu_hotplug/enable" },
  });
  kernelSystem->add_kernel_category("irq",           "IRQ Events",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/irq/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ipi/enable" },
  });
  kernelSystem->add_kernel_category("freq",          "CPU Frequency",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/power/cpu_frequency/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/power/clock_set_rate/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/power/cpu_frequency_limits/enable" },
  });
  kernelSystem->add_kernel_category("membus",        "Memory Bus Utilization",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/memory_bus/enable" },
  });
  kernelSystem->add_kernel_category("idle",          "CPU Idle",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/power/cpu_idle/enable" },
  });
  kernelSystem->add_kernel_category("disk",          "Disk I/O",
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
  kernelSystem->add_kernel_category("mmc",           "eMMC commands",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/mmc/enable" },
  });
  kernelSystem->add_kernel_category("load",          "CPU Load",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/cpufreq_interactive/enable" },
  });
  kernelSystem->add_kernel_category("sync",          "Synchronization",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/sync/enable" },
  });
  kernelSystem->add_kernel_category("workq",         "Kernel Workqueues",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/workqueue/enable" },
  });
  kernelSystem->add_kernel_category("memreclaim",    "Kernel Memory Reclaim",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_begin/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_end/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_wake/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_sleep/enable" },
  });
  kernelSystem->add_kernel_category("regulators",    "Voltage and Current Regulators",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/regulator/enable" },
  });
  kernelSystem->add_kernel_category("binder_driver", "Binder Kernel driver",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction_received/enable" },
  });
  kernelSystem->add_kernel_category("binder_lock",   "Binder global lock trace",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_lock/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_locked/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_unlock/enable" },
  });
  kernelSystem->add_kernel_category("pagecache",     "Page cache",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/filemap/enable" },
  });
}

void addAppsToTrace(const char * commaSepApps) {
  std::set<std::string> tokens;
  toolBox->parseToTokens(commaSepApps, ",", tokens);
  for (const auto & token : tokens) {
    trace->addApp(token.c_str());
  }
}

void addFunctionsToTrace(const char * commaSepFuncs) {
  std::set<std::string> tokens;
  toolBox->parseToTokens(commaSepFuncs, ",", tokens);
  for (const auto & token : tokens) {
    trace->addFunction(token.c_str());
  }
}

void addAndroidCategoriesToTrace(const char * commaSepCats) {
  std::set<std::string> tokens;
  toolBox->parseToTokens(commaSepCats, ",", tokens);
  for (const auto & token : tokens) {
      trace->addAndroidCategory(token.c_str());
  }
}

bool addCoreServicesToTrace() {
  std::set<std::string> tokens;
  if (androidSystem->has_core_services()) {
    std::string value;
    androidSystem->property_get_core_service_names(value);
    toolBox->parseToTokens(value.c_str(), ",", tokens);
    for (const auto & token : tokens) {
      trace->addApp(token.c_str());
    }
    return true;
  }
  fprintf(errorStream, "Can't enable core services - not supported\n");
  return false;
}

bool addKernelCategoriesFromFileToTrace(const char * filename) {
  std::set<std::string> tokens;
  if (!toolBox->parseFileToTokens(filename, " ", tokens)) {
    fprintf(errorStream, "error parsing category file \"%s\"\n", filename);
    return false;
  }
  for (const auto & token : tokens) {
     trace->addKernelCategory(token.c_str());
  }
  return true;
}

int main(int argc, char ** argv) {
  if (argc == 2 && 0 == strcmp(argv[1], "--help")) {
    showHelp(argv[0]);
    return EXIT_SUCCESS;
  }

  setupAndroidSystemImpl();
  setupKernelSystemImpl();
  setupDependencies();

  handleSignals = true;

  bool hasS          = false;
  bool hasAsyncStart = false;
  bool hasStream     = false;
  bool hasAsyncStop  = false;
  bool hasAsyncDump  = false;
  bool hasT          = false;
  for (;;) {
      int ret;
      int option_index = 0;
      static struct option long_options[] = {
          {"async_start",     no_argument, 0,  0 },
          {"async_stop",      no_argument, 0,  0 },
          {"async_dump",      no_argument, 0,  0 },
          {"list_categories", no_argument, 0,  0 },
          {"stream",          no_argument, 0,  0 },
          {"acore",           no_argument, 0,  0 },
          {           0,                0, 0,  0 }
      };

      ret = getopt_long(argc, argv, "a:b:cd:f:k:ns:t:zo:",
                        long_options, &option_index);

      if (ret < 0) {
          for (int i = optind; i < argc; i++) {
              trace->addKernelCategory(argv[i]);
          }
          break;
      }

      switch(ret) {
          case 'a':
              addAppsToTrace(optarg);
          break;

          case 'b':
              trace->setTraceBufferSizeKB(atoi(optarg));
          break;

          case 'c':
              trace->enableTraceOverwrite();
          break;

          case 'd':
              addAndroidCategoriesToTrace(optarg);
          break;

          case 'f':
              if (!addKernelCategoriesFromFileToTrace(optarg))
                return EXIT_FAILURE;
          break;

          case 'k':
              addFunctionsToTrace(optarg);
          break;

          case 'n':
              handleSignals = false;
          break;

          case 's':
              hasS = true;
              initSleep->setDurationSeconds(atoi(optarg));
          break;

          case 't':
              hasT = true;
              midSleep->setDurationSeconds(atoi(optarg));
          break;

          case 'z':
              dumpAction->enableCompression();
          break;

          case 'o':
              dumpAction->setOutputFile(optarg);
          break;

          case 0:
              if (!strcmp(long_options[option_index].name, "async_start")) {
                  hasAsyncStart = true;
              } else if (!strcmp(long_options[option_index].name, "async_stop")) {
                  hasAsyncStop = true;
              } else if (!strcmp(long_options[option_index].name, "async_dump")) {
                  hasAsyncDump = true;
              } else if (!strcmp(long_options[option_index].name, "stream")) {
                  hasStream = true;
              } else if (!strcmp(long_options[option_index].name, "list_categories")) {
                  listSupportedCategories();
                  return EXIT_SUCCESS;
              } else if (!strcmp(long_options[option_index].name, "acore")) {
                  if (!addCoreServicesToTrace()) {
                    return EXIT_FAILURE;
                  }
              }
          break;

          default:
              fprintf(errorStream, "\n");
              showHelp(argv[0]);
              return EXIT_FAILURE;
          break;
      }
  }

  ActionRunnerImpl actionRunnerImpl;
  if (hasS) {
    actionRunnerImpl.addAction(initSleep);
  }
  if (hasAsyncStart) {
    actionRunnerImpl.addAction(startAction);
    if (hasStream) {
      actionRunnerImpl.addAction(streamAction);
      handleSignals = true;
    }
  }
  else if (hasAsyncStop) {
    actionRunnerImpl.addAction(stopAction);
    actionRunnerImpl.addAction(dumpAction);
    actionRunnerImpl.addAction(cleanUpAction);
  }
  else if (hasAsyncDump) {
    actionRunnerImpl.addAction(dumpAction);
  }
  else {
    actionRunnerImpl.addAction(startAction);
    actionRunnerImpl.addAction(midSleep);
    if (hasStream) {
      actionRunnerImpl.addAction(streamAction);
    }
    actionRunnerImpl.addAction(stopAction);
    actionRunnerImpl.addAction(dumpAction);
    actionRunnerImpl.addAction(cleanUpAction);
  }

  if (handleSignals)
    registerSigHandler();

  bool ok = actionRunnerImpl.tryRunActions();

  delete initSleep;
  delete startAction;
  delete midSleep;
  delete streamAction;
  delete stopAction;
  delete dumpAction;
  delete cleanUpAction;
  if (ok)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}
