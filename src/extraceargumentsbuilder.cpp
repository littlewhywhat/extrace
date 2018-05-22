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

#include "extraceargumentsbuilder.h"
#include "androidtoolbox.h"

static const char * HELP_OPTION_NAME = "Help";
static const char * CIRCLE_BUFFER_OPTION_NAME = "CircleBuffer";
static const char * IGNORE_SIGNALS_OPTION_NAME = "IgnoreSignals";
static const char * COMPRESSED_OPTION_NAME = "Compressed";
static const char * ASYNC_START_OPTION_NAME = "AsyncStart";
static const char * ASYNC_STOP_OPTION_NAME = "AsyncStop";
static const char * ASYNC_DUMP_OPTION_NAME = "AsyncDump";
static const char * STREAM_OPTION_NAME = "Stream";
static const char * LIST_CATEGORIES_OPTION_NAME = "ListCategories";
static const char * CORE_SERVICES_OPTION_NAME = "CoreServices";
static const char * KERNEL_CATEG_FILE_OPTION_NAME = "KernelCategoriesFilename";
static const char * OUT_FILE_OPTION_NAME = "OutputFile";
static const char * BUFFER_SIZE_OPTION_NAME = "BufferSize";
static const char * INIT_SLEEP_OPTION_NAME = "InitSleep";
static const char * MID_SLEEP_OPTION_NAME = "MidSleep";
static const char * APPS_OPTION_NAME = "Apps";
static const char * ANDROID_CATEG_OPTION_NAME = "AndroidCategories";
static const char * KERNEL_CATEG_OPTION_NAME = "KernelCategories";
static const char * KERNEL_FUNC_OPTION_NAME = "KernelFunctions";
static const char * PERIOD_OPTION_NAME = "Period";
static const char * TIMES_OPTION_NAME = "Times";
static const char * PIDS_OPTION_NAME = "PIDs";
static const char * CPU_OPTION_NAME = "CPULimit";
static const char * USS_OPTION_NAME = "USSLimit";
static const char * INTERPRET_OPTION_NAME = "Interpret";

static const string HELP_MESSAGE = "usage: %s [options]\n"
         "options include:\n"
         "  -a app_package  enable app-level tracing for a comma\n"
         "                  separated list of package names\n"
         "  -b N            use a trace buffer size of N KB\n"
         "  -c              trace into a circular buffer\n"
         "  -d aname,...    trace the listed android categories\n"
         "  -e kname,...    trace the listed kernel categories\n"
         "  -f filename     use the categories written in a file as space-separated\n"
         "                    values in a line\n"
         "  -p N            period of memory measurement\n"
         "  -m N            number of periods to pass\n"
         "  -pids N,...     PIDs to take memory measurement of\n"
         "  -minCpu N       minimal CPU usage that should trace record have to display to user\n"
         "  -minUss N       minimal Uss that should a trace record have to be displayed to user\n"
         "  -n              ignore signals\n"
         "  -s N            sleep for N seconds before tracing [default 0]\n"
         "  -t N            trace for N seconds [default 5]\n"
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
         " --interpret      interpret dump file only\n";

ExtraceArgumentsBuilder::ExtraceArgumentsBuilder() {
  m_AndroidTraceCategories["gfx"]          = Android::TraceCategory::GRAPHICS         ;
  m_AndroidTraceCategories["input"]        = Android::TraceCategory::INPUT            ;
  m_AndroidTraceCategories["view"]         = Android::TraceCategory::VIEW             ;
  m_AndroidTraceCategories["webview"]      = Android::TraceCategory::WEBVIEW          ;
  m_AndroidTraceCategories["wm"]           = Android::TraceCategory::WINDOW_MANAGER   ;
  m_AndroidTraceCategories["am"]           = Android::TraceCategory::ACTIVITY_MANAGER ;
  m_AndroidTraceCategories["sm"]           = Android::TraceCategory::SYNC_MANAGER     ;
  m_AndroidTraceCategories["audio"]        = Android::TraceCategory::AUDIO            ;
  m_AndroidTraceCategories["video"]        = Android::TraceCategory::VIDEO            ;
  m_AndroidTraceCategories["camera"]       = Android::TraceCategory::CAMERA           ;
  m_AndroidTraceCategories["hal"]          = Android::TraceCategory::HAL              ;
  m_AndroidTraceCategories["app"]          = Android::TraceCategory::APP              ;
  m_AndroidTraceCategories["res"]          = Android::TraceCategory::RESOURCES        ;
  m_AndroidTraceCategories["dalvik"]       = Android::TraceCategory::DALVIK           ;
  m_AndroidTraceCategories["rs"]           = Android::TraceCategory::RS               ;
  m_AndroidTraceCategories["bionic"]       = Android::TraceCategory::BIONIC           ;
  m_AndroidTraceCategories["power"]        = Android::TraceCategory::POWER            ;
  m_AndroidTraceCategories["pm"]           = Android::TraceCategory::PACKAGE_MANAGER  ;
  m_AndroidTraceCategories["ss"]           = Android::TraceCategory::SYSTEM_SERVER    ;
  m_AndroidTraceCategories["database"]     = Android::TraceCategory::DATABASE         ;
  m_AndroidTraceCategories["network"]      = Android::TraceCategory::NETWORK          ;

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

void ExtraceArgumentsBuilder::registerCmdLineOpts(CmdLineArgsParser & cmdLineArgsParser) const {
  cmdLineArgsParser.register_boolean("--help", HELP_OPTION_NAME);
  cmdLineArgsParser.register_boolean("-c", CIRCLE_BUFFER_OPTION_NAME);
  cmdLineArgsParser.register_boolean("-n", IGNORE_SIGNALS_OPTION_NAME);
  cmdLineArgsParser.register_boolean("-z", COMPRESSED_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--async_start", ASYNC_START_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--async_stop", ASYNC_STOP_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--async_dump", ASYNC_DUMP_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--stream", STREAM_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--list_categories", LIST_CATEGORIES_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--acore", CORE_SERVICES_OPTION_NAME);
  cmdLineArgsParser.register_boolean("--interpret", INTERPRET_OPTION_NAME);
  cmdLineArgsParser.register_string("-f", KERNEL_CATEG_FILE_OPTION_NAME);
  cmdLineArgsParser.register_string("-o", OUT_FILE_OPTION_NAME);
  cmdLineArgsParser.register_integer("-b", BUFFER_SIZE_OPTION_NAME);
  cmdLineArgsParser.register_integer("-s", INIT_SLEEP_OPTION_NAME);
  cmdLineArgsParser.register_integer("-t", MID_SLEEP_OPTION_NAME);
  cmdLineArgsParser.register_integer("-p", PERIOD_OPTION_NAME);
  cmdLineArgsParser.register_integer("-m", TIMES_OPTION_NAME);
  cmdLineArgsParser.register_integer("-minCpu", CPU_OPTION_NAME);
  cmdLineArgsParser.register_integer("-minUss", USS_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepIntList("-pids", PIDS_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-a", APPS_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-d", ANDROID_CATEG_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-e", KERNEL_CATEG_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-k", KERNEL_FUNC_OPTION_NAME);
}

ExtraceArguments * ExtraceArgumentsBuilder::createHelpExtraceArguments(const string & appName) const {
  ExtraceArguments * traceArguments = new ExtraceArguments();
  traceArguments->setAppName(appName);
  traceArguments->setHelpMessage(getHelpMessage());
  return traceArguments;
}

bool ExtraceArgumentsBuilder::tryPutCategoriesFromFile(ExtraceArguments * extraceArguments,
                                                       const string & filename) const {
  set<string> tokens;
  if (!AndroidToolBox().parseFileToTokens(filename.c_str(), " ", tokens)) {
    // TODO fprintf(m_Wire.getErrorStream(), "tryPutCategoriesFromFile - error parsing category file \"%s\"\n", filename.c_str());
    return false;
  }
  for (const auto & token : tokens) {
    if (m_KernelTraceCategories.find(token) == m_KernelTraceCategories.cend()) {
      return false;
    }
    extraceArguments->addKernelCategory(m_KernelTraceCategories.at(token));
  }
  return true;
}

ExtraceArguments * ExtraceArgumentsBuilder::createExtraceArguments(const Arguments & arguments) const {
  ExtraceArguments * traceArguments = new ExtraceArguments();
  if (arguments.has_integer(CPU_OPTION_NAME)) {
    traceArguments->setCpuLimit(arguments.get_integer(CPU_OPTION_NAME));
  }
  if (arguments.has_integer(USS_OPTION_NAME)) {
    traceArguments->setUssLimit(arguments.get_integer(USS_OPTION_NAME) * 1024);
  }
  if (arguments.has_integer(PERIOD_OPTION_NAME)) {
    traceArguments->setPeriod(arguments.get_integer(PERIOD_OPTION_NAME));
  }
  if (arguments.has_integer(TIMES_OPTION_NAME)) {
    traceArguments->setTimes(arguments.get_integer(TIMES_OPTION_NAME));
  }
  if (arguments.hasIntList(PIDS_OPTION_NAME)) {
    traceArguments->setPIDs(arguments.getIntList(PIDS_OPTION_NAME));
  }
  if (arguments.is_enabled(HELP_OPTION_NAME)) {
    traceArguments->setHelpMessage(getHelpMessage());
  }
  if (arguments.is_enabled(CIRCLE_BUFFER_OPTION_NAME)) {
    traceArguments->enableCircleBuffer();
  }
  if (arguments.is_enabled(IGNORE_SIGNALS_OPTION_NAME)) {
    traceArguments->enableIgnoreSignals();
  }
  if (arguments.is_enabled(COMPRESSED_OPTION_NAME)) {
    traceArguments->enableCompression();
  }
  if (arguments.is_enabled(ASYNC_START_OPTION_NAME)) {
    traceArguments->enableAsyncStart();
  }
  if (arguments.is_enabled(ASYNC_STOP_OPTION_NAME)) {
    traceArguments->enableAsyncStop();
  }
  if (arguments.is_enabled(ASYNC_DUMP_OPTION_NAME)) {
    traceArguments->enableAsyncDump();
  }
  if (arguments.is_enabled(STREAM_OPTION_NAME)) {
    traceArguments->enableStream();
  }
  if (arguments.is_enabled(LIST_CATEGORIES_OPTION_NAME)) {
    traceArguments->enableListCategories();
  }
  if (arguments.is_enabled(CORE_SERVICES_OPTION_NAME)) {
    traceArguments->enableCoreServices();
  }
  if (arguments.is_enabled(INTERPRET_OPTION_NAME)) {
    traceArguments->enableInterpret();
  }
  if (arguments.has_string(KERNEL_CATEG_FILE_OPTION_NAME)) {
    if (!tryPutCategoriesFromFile(traceArguments,
                                  arguments.get_string(KERNEL_CATEG_FILE_OPTION_NAME))) {
      delete traceArguments;
      return NULL;
    }
  }
  if (arguments.has_string(OUT_FILE_OPTION_NAME)) {
    auto & filename = arguments.get_string(OUT_FILE_OPTION_NAME);
    traceArguments->setOutputFilename(filename);
  }
  if (arguments.has_integer(BUFFER_SIZE_OPTION_NAME)) {
    traceArguments->setBufferSize(arguments.get_integer(BUFFER_SIZE_OPTION_NAME));
  }
  if (arguments.has_integer(INIT_SLEEP_OPTION_NAME)) {
    const auto & duration = arguments.get_integer(INIT_SLEEP_OPTION_NAME);
    traceArguments->setInitSleepDuration(duration);
  }
  if (arguments.has_integer(MID_SLEEP_OPTION_NAME)) {
    const auto & duration = arguments.get_integer(MID_SLEEP_OPTION_NAME);
    traceArguments->setMidSleepDuration(duration);
  }
  if (arguments.hasStringList(APPS_OPTION_NAME)) {
    const auto & list = arguments.getStringList(APPS_OPTION_NAME);
    traceArguments->setApps(list);
  }
  if (arguments.hasStringList(ANDROID_CATEG_OPTION_NAME)) {
    // TODO consider put list supported categories instead
    const auto & list = arguments.getStringList(ANDROID_CATEG_OPTION_NAME);
    for (const auto & androidCategoryName : list) {
      if (m_AndroidTraceCategories.find(androidCategoryName) != m_AndroidTraceCategories.cend()) {
        traceArguments->addAndroidCategory(m_AndroidTraceCategories.at(androidCategoryName));
      }
      else {
        delete traceArguments;
        return NULL;
      }
    }
  }
  if (arguments.hasStringList(KERNEL_CATEG_OPTION_NAME)) {
    const auto & list = arguments.getStringList(KERNEL_CATEG_OPTION_NAME);
    for (const auto & kernelCategoryName : list) {
      if (m_KernelTraceCategories.find(kernelCategoryName) != m_KernelTraceCategories.cend()) {
        traceArguments->addKernelCategory(m_KernelTraceCategories.at(kernelCategoryName));
      }
      else {
        delete traceArguments;
        return NULL;
      }
    }
  }
  if (arguments.hasStringList(KERNEL_FUNC_OPTION_NAME)) {
    const auto & list = arguments.getStringList(KERNEL_FUNC_OPTION_NAME);
    traceArguments->setKernelFunctions(list);
  }
  return traceArguments;
}

const ExtraceArguments * ExtraceArgumentsBuilder::build(const Wire & wire, const CmdLineArgs & cmdLineArgs) const {
  CmdLineArgsParser cmdLineArgsParser(make_shared<AndroidToolBox>());
  registerCmdLineOpts(cmdLineArgsParser);
  Arguments arguments;
  if (cmdLineArgsParser.parse(arguments, cmdLineArgs) != cmdLineArgs.getCount()) {
    return createHelpExtraceArguments(cmdLineArgs.getAppName());
  }
  auto * traceArguments = createExtraceArguments(arguments);
  if (!traceArguments) {
    return createHelpExtraceArguments(cmdLineArgs.getAppName());
  }
  traceArguments->setAppName(cmdLineArgs.getAppName());
  if (traceArguments->hasHelpMessage()
       || traceArguments->listCategoriesEnabled()
       || traceArguments->hasKernelCategories()
       || traceArguments->hasAndroidCategories()
       || traceArguments->hasApps()
       || traceArguments->hasKernelFunctions()
       || traceArguments->coreServicesEnabled()
       || traceArguments->asyncStopEnabled()
       || traceArguments->asyncDumpEnabled()
       || (traceArguments->interpretEnabled()
           && traceArguments->hasPIDs()
           && traceArguments->hasOutputFilename())) {
    return traceArguments;
  }
  return createHelpExtraceArguments(cmdLineArgs.getAppName());
}

const string & ExtraceArgumentsBuilder::getHelpMessage() const {
  return HELP_MESSAGE;
}
