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
#include "extracearguments.h"

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

void ExtraceArguments::setArguments(const Arguments * arguments) {
  m_Arguments = arguments;
}

void ExtraceArguments::registerTo(ArgsParser & argsParser) const {
  argsParser.register_boolean("--help", HELP_OPTION_NAME);
  argsParser.register_boolean("-c", CIRCLE_BUFFER_OPTION_NAME);
  argsParser.register_boolean("-n", IGNORE_SIGNALS_OPTION_NAME);
  argsParser.register_boolean("-z", COMPRESSED_OPTION_NAME);
  argsParser.register_boolean("--async_start", ASYNC_START_OPTION_NAME);
  argsParser.register_boolean("--async_stop", ASYNC_STOP_OPTION_NAME);
  argsParser.register_boolean("--async_dump", ASYNC_DUMP_OPTION_NAME);
  argsParser.register_boolean("--stream", STREAM_OPTION_NAME);
  argsParser.register_boolean("--list_categories", LIST_CATEGORIES_OPTION_NAME);
  argsParser.register_boolean("--acore", CORE_SERVICES_OPTION_NAME);
  argsParser.register_string("-f", KERNEL_CATEG_FILE_OPTION_NAME);
  argsParser.register_string("-o", OUT_FILE_OPTION_NAME);
  argsParser.register_integer("-b", BUFFER_SIZE_OPTION_NAME);
  argsParser.register_integer("-s", INIT_SLEEP_OPTION_NAME);
  argsParser.register_integer("-t", MID_SLEEP_OPTION_NAME);
  argsParser.registerCommaSepList("-a", APPS_OPTION_NAME);
  argsParser.registerCommaSepList("-d", ANDROID_CATEG_OPTION_NAME);
  argsParser.registerCommaSepList("-e", KERNEL_CATEG_OPTION_NAME);
  argsParser.registerCommaSepList("-k", KERNEL_FUNC_OPTION_NAME);
}

bool ExtraceArguments::areCorrect() const {
  return haveHelpOption()
         || haveListCategoriesOption()
         || haveKernelCategoryFilename()
         || haveKernelCategories()
         || haveAndroidCategories()
         || haveApps()
         || haveKernelFunctions();
}

void ExtraceArguments::printHelp(FILE* out, const string & appName) const {
  fprintf(out, "usage: %s [options]\n", appName.c_str());
  fprintf(out, "options include:\n"
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

bool ExtraceArguments::haveHelpOption() const {
  return m_Arguments->is_enabled(HELP_OPTION_NAME);
}

bool ExtraceArguments::haveListCategoriesOption() const {
  return m_Arguments->is_enabled(LIST_CATEGORIES_OPTION_NAME);
}

const string & ExtraceArguments::getDumpFilename() const {
  return m_Arguments->get_string(OUT_FILE_OPTION_NAME);
}

bool ExtraceArguments::haveDumpFilename() const {
  return m_Arguments->has_string(OUT_FILE_OPTION_NAME);
}

bool ExtraceArguments::enableCompression() const {
  return m_Arguments->is_enabled(COMPRESSED_OPTION_NAME);
}

bool ExtraceArguments::enableAsyncStart() const {
  return m_Arguments->is_enabled(ASYNC_START_OPTION_NAME);
}

bool ExtraceArguments::enableAsyncStop() const {
  return m_Arguments->is_enabled(ASYNC_STOP_OPTION_NAME);
}

bool ExtraceArguments::enableAsyncDump() const {
  return m_Arguments->is_enabled(ASYNC_DUMP_OPTION_NAME);
}

bool ExtraceArguments::enableStream() const {
  return m_Arguments->is_enabled(STREAM_OPTION_NAME);
}

bool ExtraceArguments::enableListCategories() const {
  return m_Arguments->is_enabled(LIST_CATEGORIES_OPTION_NAME);
}

bool ExtraceArguments::enableCoreServices() const {
  return m_Arguments->is_enabled(CORE_SERVICES_OPTION_NAME);
}

bool ExtraceArguments::enableCircleBuffer() const {
  return m_Arguments->is_enabled(CIRCLE_BUFFER_OPTION_NAME);
}

bool ExtraceArguments::enableSignalsIgnore() const {
  return m_Arguments->is_enabled(IGNORE_SIGNALS_OPTION_NAME);
}

bool ExtraceArguments::haveKernelCategoryFilename() const {
  return m_Arguments->has_string(KERNEL_CATEG_FILE_OPTION_NAME);
}

const string & ExtraceArguments::getKernelCategoryFilename() const {
  return m_Arguments->get_string(KERNEL_CATEG_FILE_OPTION_NAME);  
}

bool ExtraceArguments::specifyBufferSize() const {
  return m_Arguments->has_integer(BUFFER_SIZE_OPTION_NAME);
}

int ExtraceArguments::getBufferSize() const {
  return m_Arguments->get_integer(BUFFER_SIZE_OPTION_NAME);
}

bool ExtraceArguments::specifyInitSleepDuration() const {
  return m_Arguments->has_integer(INIT_SLEEP_OPTION_NAME);
}

int ExtraceArguments::getInitSleepDuration() const {
  return m_Arguments->get_integer(INIT_SLEEP_OPTION_NAME);
}

bool ExtraceArguments::specifyMidSleepDuration() const {
  return m_Arguments->has_integer(MID_SLEEP_OPTION_NAME);
}

int ExtraceArguments::getMidSleepDuration() const {
  return m_Arguments->get_integer(MID_SLEEP_OPTION_NAME);
}

bool ExtraceArguments::haveApps() const {
  return m_Arguments->hasStringList(APPS_OPTION_NAME);
}

const vector<string> & ExtraceArguments::getApps() const {
  return m_Arguments->getStringList(APPS_OPTION_NAME);
}

bool ExtraceArguments::haveAndroidCategories() const {
  return m_Arguments->hasStringList(ANDROID_CATEG_OPTION_NAME);
}

const vector<string> & ExtraceArguments::getAndroidCategories() const {
  return m_Arguments->getStringList(ANDROID_CATEG_OPTION_NAME);
}

bool ExtraceArguments::haveKernelCategories() const {
  return m_Arguments->hasStringList(KERNEL_CATEG_OPTION_NAME);
}

const vector<string> & ExtraceArguments::getKernelCategories() const {
  return m_Arguments->getStringList(KERNEL_CATEG_OPTION_NAME);
}

bool ExtraceArguments::haveKernelFunctions() const {
  return m_Arguments->hasStringList(KERNEL_FUNC_OPTION_NAME);
}

const vector<string> & ExtraceArguments::getKernelFunctions() const {
  return m_Arguments->getStringList(KERNEL_FUNC_OPTION_NAME);
}
