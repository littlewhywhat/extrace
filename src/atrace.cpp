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
#include "atraceapp.h"
#include "androidtoolbox.h"
#include "trace_impl.h"

#include <getopt.h>      // getopt_long, no_argument
#include <signal.h>      // sigaction
#include <utils/Trace.h> // ATRACE_TAGs
#include <stdlib.h>      // EXIT_FAILURE, EXIT_SUCCESS

AtraceApp atrace;
FILE * errstream = stderr;

void handleSignal(int /*signo*/)
{
    atrace.handleSignal();
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

void listSupportedCategories(KernelSystem * kernel_system, AndroidSystem * android_system) {
  const auto & kernelCategories = kernel_system->getCategories();
  for (const auto & category : kernelCategories) {
      if (kernel_system->isCategorySupported(category)) {
          printf("  %10s - %s\n", category.name, category.longname);
      }
  }
  const auto & androidCategories = android_system->getCategories();
  for (const auto & category : androidCategories) {
      printf("  %10s - %s\n", category.name, category.longname);
  }
}

// Print the command usage help to errstream.
void showHelp(const char *cmd)
{
    fprintf(errstream, "usage: %s [options] [categories...]\n", cmd);
    fprintf(errstream, "options include:\n"
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

int main(int argc, char ** argv) {
  Toolbox * toolbox = new AndroidToolbox();
  FileSystemImpl * file_system = new FileSystemImpl();
  file_system->set_errstream(errstream);
  KernelSystemImpl * kernel_system_impl = new KernelSystemImpl();
  kernel_system_impl->set_errstream(errstream);
  kernel_system_impl->set_file_system(file_system);
  kernel_system_impl->set_toolbox(toolbox);
  kernel_system_impl->set_systime(new SystemTimeImpl());
  AndroidSystemImpl * android_system_impl = new AndroidSystemImpl();
  android_system_impl->set_errstream(errstream);

  auto sp_android_system_impl = shared_ptr<AndroidSystem>(android_system_impl);
  auto sp_kernel_system_impl  = shared_ptr<KernelSystem>(kernel_system_impl);

  TraceImpl * trace = new TraceImpl();
  trace->setErrorStream(errstream);
  trace->setKernelSystem(sp_kernel_system_impl);
  trace->setAndroidSystem(sp_android_system_impl);

  atrace.setTrace(trace);
  atrace.set_kernel_system(sp_kernel_system_impl);
  atrace.set_android_system(sp_android_system_impl);
  atrace.set_errstream(errstream);
  atrace.set_outstream(stdout);
  android_system_impl->add_category( "gfx",        "Graphics",         ATRACE_TAG_GRAPHICS         );
  android_system_impl->add_category( "input",      "Input",            ATRACE_TAG_INPUT            );
  android_system_impl->add_category( "view",       "View System",      ATRACE_TAG_VIEW             );
  android_system_impl->add_category( "webview",    "WebView",          ATRACE_TAG_WEBVIEW          );
  android_system_impl->add_category( "wm",         "Window Manager",   ATRACE_TAG_WINDOW_MANAGER   );
  android_system_impl->add_category( "am",         "Activity Manager", ATRACE_TAG_ACTIVITY_MANAGER );
  android_system_impl->add_category( "sm",         "Sync Manager",     ATRACE_TAG_SYNC_MANAGER     );
  android_system_impl->add_category( "audio",      "Audio",            ATRACE_TAG_AUDIO            );
  android_system_impl->add_category( "video",      "Video",            ATRACE_TAG_VIDEO            );
  android_system_impl->add_category( "camera",     "Camera",           ATRACE_TAG_CAMERA           );
  android_system_impl->add_category( "hal",        "Hardware Modules", ATRACE_TAG_HAL              );
  android_system_impl->add_category( "app",        "Application",      ATRACE_TAG_APP              );
  android_system_impl->add_category( "res",        "Resource Loading", ATRACE_TAG_RESOURCES        );
  android_system_impl->add_category( "dalvik",     "Dalvik VM",        ATRACE_TAG_DALVIK           );
  android_system_impl->add_category( "rs",         "RenderScript",     ATRACE_TAG_RS               );
  android_system_impl->add_category( "bionic",     "Bionic C Library", ATRACE_TAG_BIONIC           );
  android_system_impl->add_category( "power",      "Power Management", ATRACE_TAG_POWER            );
  android_system_impl->add_category( "pm",         "Package Manager",  ATRACE_TAG_PACKAGE_MANAGER  );
  android_system_impl->add_category( "ss",         "System Server",    ATRACE_TAG_SYSTEM_SERVER    );
  android_system_impl->add_category( "database",   "Database",         ATRACE_TAG_DATABASE         );
  android_system_impl->add_category( "network",    "Network",          ATRACE_TAG_NETWORK          );

  kernel_system_impl->add_kernel_category("sched",         "CPU Scheduling",
  {
     { EnableFile::REQ, "/sys/kernel/debug/tracing/events/sched/sched_switch/enable" },
     { EnableFile::REQ, "/sys/kernel/debug/tracing/events/sched/sched_wakeup/enable" },
     { EnableFile::OPT, "/sys/kernel/debug/tracing/events/sched/sched_blocked_reason/enable" },
     { EnableFile::OPT, "/sys/kernel/debug/tracing/events/sched/sched_cpu_hotplug/enable" },
  });
  kernel_system_impl->add_kernel_category("irq",           "IRQ Events",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/irq/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/ipi/enable" },
  });
  kernel_system_impl->add_kernel_category("freq",          "CPU Frequency",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/power/cpu_frequency/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/power/clock_set_rate/enable" },
     { EnableFile::OPT,      "/sys/kernel/debug/tracing/events/power/cpu_frequency_limits/enable" },
  });
  kernel_system_impl->add_kernel_category("membus",        "Memory Bus Utilization",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/memory_bus/enable" },
  });
  kernel_system_impl->add_kernel_category("idle",          "CPU Idle",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/power/cpu_idle/enable" },
  });
  kernel_system_impl->add_kernel_category("disk",          "Disk I/O",
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
  kernel_system_impl->add_kernel_category("mmc",           "eMMC commands",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/mmc/enable" },
  });
  kernel_system_impl->add_kernel_category("load",          "CPU Load",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/cpufreq_interactive/enable" },
  });
  kernel_system_impl->add_kernel_category("sync",          "Synchronization",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/sync/enable" },
  });
  kernel_system_impl->add_kernel_category("workq",         "Kernel Workqueues",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/workqueue/enable" },
  });
  kernel_system_impl->add_kernel_category("memreclaim",    "Kernel Memory Reclaim",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_begin/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_direct_reclaim_end/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_wake/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/vmscan/mm_vmscan_kswapd_sleep/enable" },
  });
  kernel_system_impl->add_kernel_category("regulators",    "Voltage and Current Regulators",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/regulator/enable" },
  });
  kernel_system_impl->add_kernel_category("binder_driver", "Binder Kernel driver",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_transaction_received/enable" },
  });
  kernel_system_impl->add_kernel_category("binder_lock",   "Binder global lock trace",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_lock/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_locked/enable" },
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/binder/binder_unlock/enable" },
  });
  kernel_system_impl->add_kernel_category("pagecache",     "Page cache",
  {
     { EnableFile::REQ,      "/sys/kernel/debug/tracing/events/filemap/enable" },
  });

  if (argc == 2 && 0 == strcmp(argv[1], "--help")) {
    showHelp(argv[0]);
    return EXIT_SUCCESS;
  }

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

      std::set<std::string> tokens;
      switch(ret) {
          case 'a':
              toolbox->parseToTokens(optarg, ",", tokens);
              for (const auto & token : tokens) {
                trace->addApp(token.c_str());
              }
              tokens.clear();    
          break;

          case 'b':
              trace->setTraceBufferSizeKB(atoi(optarg));
          break;

          case 'c':
              trace->enableTraceOverwrite();
          break;

          case 'd':
              toolbox->parseToTokens(optarg, ",", tokens);
              for (const auto & androidCategory : tokens) {
                  trace->addAndroidCategory(androidCategory.c_str());
              }
              tokens.clear();
          break;

          case 'f':
              if (!toolbox->parseFileToTokens(optarg, " ", tokens)) {
                fprintf(errstream, "error parsing category file \"%s\"\n", optarg);
                return EXIT_FAILURE;
              }
              for (const auto & token : tokens) {
                 trace->addKernelCategory(token.c_str());
              }
              tokens.clear();
          break;

          case 'k':
              toolbox->parseToTokens(optarg, ",", tokens);
              for (const auto & token : tokens) {
                trace->addFunction(token.c_str());
              }
              tokens.clear();
          break;

          case 'n':
              atrace.nosignals();
          break;

          case 's':
              atrace.set_initialSleepSecs(atoi(optarg));
          break;

          case 't':
              atrace.set_traceDurationSeconds(atoi(optarg));
          break;

          case 'z':
              atrace.enable_compression();
          break;

          case 'o':
              atrace.set_outputFile(optarg);
          break;

          case 0:
              if (!strcmp(long_options[option_index].name, "async_start")) {
                  atrace.set_async(true);
                  atrace.set_stop(false);
                  atrace.set_dump(false);
                  trace->enableTraceOverwrite();
              } else if (!strcmp(long_options[option_index].name, "async_stop")) {
                  atrace.set_async(true);
                  atrace.set_start(false);
              } else if (!strcmp(long_options[option_index].name, "async_dump")) {
                  atrace.set_async(true);
                  atrace.set_start(false);
                  atrace.set_stop(false);
              } else if (!strcmp(long_options[option_index].name, "stream")) {
                  atrace.enable_streaming();
                  atrace.set_dump(false);
              } else if (!strcmp(long_options[option_index].name, "list_categories")) {
                  listSupportedCategories(kernel_system_impl, android_system_impl);
                  return EXIT_SUCCESS;
              } else if (!strcmp(long_options[option_index].name, "acore")) {
                if (android_system_impl->has_core_services()) {
                  std::string value;
                  android_system_impl->property_get_core_service_names(value);
                  toolbox->parseToTokens(value.c_str(), ",", tokens);
                  for (const auto & token : tokens) {
                    trace->addApp(token.c_str());
                  }
                  tokens.clear();
                } else {
                  fprintf(errstream, "Can't enable core services - not supported\n");
                }
              }
          break;

          default:
              fprintf(errstream, "\n");
              showHelp(argv[0]);
              return EXIT_FAILURE;
          break;
      }
  }

  registerSigHandler();

  return atrace.run()? EXIT_SUCCESS : EXIT_FAILURE;
}
