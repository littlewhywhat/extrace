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
#include "extraceconsoleapp.h"

void ExtraceConsoleApp::register_args_to(ArgsParser & argsParser) const
{
  argsParser.register_boolean("not_defined", "not_defined");
  // TODO
  // argsParser.register_boolean("--async_start");
}

bool ExtraceConsoleApp::admits(const Arguments & arguments) const
{
  arguments.is_enabled("not_defined");
  // TODO difficult arguments
  // if (async_start && async_stop && async_continue)
  return true;
}

void ExtraceConsoleApp::setup_with(const Arguments & arguments)
{
  arguments.is_enabled("not_defined");
  // TODO
  // this->traceSession.set_buffer_size(arguments.get_integer(BUFFER_SIZE_ID));
}

void ExtraceConsoleApp::do_run()
{
  // TODO
  // MachineTrace trace;
  // this->traceSession.run(trace);
}

bool ExtraceConsoleApp::do_succeeded(void) const 
{ 
  // TODO
  // return !this->TraceSession.report.is_empty();
  return true;
}
