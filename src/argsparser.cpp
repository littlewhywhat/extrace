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

#include "argsparser.h"

#include "stdio.h"

bool ArgsParser::is_boolean_spec(const char * spec)
{
  return this->boolean_opt_names.find(spec) != this->boolean_opt_names.end();
}

bool ArgsParser::is_integer_spec(const char * spec)
{
  return this->integer_opt_names.find(spec) != this->integer_opt_names.end();
}

bool ArgsParser::is_string_spec(const char * spec)
{
  return this->string_opt_names.find(spec) != this->string_opt_names.end();
}

void ArgsParser::register_boolean(const char * specifier, const char * option_name)
{
  this->boolean_opt_names[specifier] = option_name;
}

void ArgsParser::register_integer(const char * specifier, const char * option_name)
{
  this->integer_opt_names[specifier] = option_name;
}

void ArgsParser::register_string(const char * specifier, const char * option_name)
{
  this->string_opt_names[specifier] = option_name;
}

int ArgsParser::parse(Arguments & arguments, int argc, const char ** argv)
{
  int i = 1;
  while (i < argc)
  {
    const char * arg = argv[i];
    if (is_boolean_spec(arg))
    {
      arguments.put_boolean(this->boolean_opt_names[arg].c_str());
      i++;
      continue;
    }
    if ((i+1) == argc)
    {
      break;
    }
    if (is_integer_spec(arg))
    {
      arguments.put_integer(this->integer_opt_names[arg].c_str(), std::stoi(argv[i+1]));
    }
    else if (is_string_spec(arg))
    {
      arguments.put_string(this->string_opt_names[arg].c_str(), argv[i+1]);
    }
    else
    {
      break;
    }
    i+=2;
  }
  return i;
}