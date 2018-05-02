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
static const string HELP_MESSAGE = "usage: %s [options]\n"
         "options include:\n"
         "  -a appname      enable app-level tracing for a comma\n"
         "                  separated list of cmdlines\n"
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
         " --acore          add core services.\n";

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
  cmdLineArgsParser.register_string("-f", KERNEL_CATEG_FILE_OPTION_NAME);
  cmdLineArgsParser.register_string("-o", OUT_FILE_OPTION_NAME);
  cmdLineArgsParser.register_integer("-b", BUFFER_SIZE_OPTION_NAME);
  cmdLineArgsParser.register_integer("-s", INIT_SLEEP_OPTION_NAME);
  cmdLineArgsParser.register_integer("-t", MID_SLEEP_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-a", APPS_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-d", ANDROID_CATEG_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-e", KERNEL_CATEG_OPTION_NAME);
  cmdLineArgsParser.registerCommaSepList("-k", KERNEL_FUNC_OPTION_NAME);
}

ExtraceArguments * ExtraceArgumentsBuilder::createHelpExtraceArguments() const {
  ExtraceArguments * traceArguments = new ExtraceArguments();
  traceArguments->setHelpMessage(getHelpMessage());
  return traceArguments;
}

ExtraceArguments * ExtraceArgumentsBuilder::createExtraceArguments(const Arguments & arguments) const {
  ExtraceArguments * traceArguments = new ExtraceArguments();
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
  if (arguments.has_string(KERNEL_CATEG_FILE_OPTION_NAME)) {
    auto & filename = arguments.get_string(KERNEL_CATEG_FILE_OPTION_NAME);
    traceArguments->setKernelCategoryFilename(filename);
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
    const auto & list = arguments.getStringList(ANDROID_CATEG_OPTION_NAME);
    traceArguments->setAndroidCategories(list);
  }
  if (arguments.hasStringList(KERNEL_CATEG_OPTION_NAME)) {
    const auto & list = arguments.getStringList(KERNEL_CATEG_OPTION_NAME);
    traceArguments->setKernelCategories(list);
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
    return createHelpExtraceArguments();
  }
  auto * traceArguments = createExtraceArguments(arguments);
  traceArguments->setAppName(cmdLineArgs.getAppName());
  if (traceArguments->hasHelpMessage()
       || traceArguments->listCategoriesEnabled()
       || traceArguments->hasKernelCategoryFilename()
       || traceArguments->hasKernelCategories()
       || traceArguments->hasAndroidCategories()
       || traceArguments->hasApps()
       || traceArguments->hasKernelFunctions()) {
    return traceArguments;
  }
  traceArguments->setHelpMessage(getHelpMessage());
  return traceArguments;
}

const string & ExtraceArgumentsBuilder::getHelpMessage() const {
  return HELP_MESSAGE;
}
