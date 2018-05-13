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

#include "cmdlineargsparser.h"

CmdLineArgsParser::CmdLineArgsParser(const shared_ptr<ToolBox> & toolBox) {
  m_ToolBox = toolBox;
}

bool CmdLineArgsParser::is_boolean_spec(const char * spec)
{
  return this->boolean_opt_names.find(spec) != this->boolean_opt_names.end();
}

bool CmdLineArgsParser::is_integer_spec(const char * spec)
{
  return this->integer_opt_names.find(spec) != this->integer_opt_names.end();
}

bool CmdLineArgsParser::is_string_spec(const char * spec)
{
  return this->string_opt_names.find(spec) != this->string_opt_names.end();
}

bool CmdLineArgsParser::isCommaSepIntListSpec(const char * specifier) const {
  return myCommaSepIntListOptNames.find(specifier) != myCommaSepIntListOptNames.end();
}

bool CmdLineArgsParser::isCommaSepListSpec(const char * specifier) const {
  return m_CommaSepListOptNames.find(specifier) != m_CommaSepListOptNames.end();
}

void CmdLineArgsParser::register_boolean(const char * specifier, const char * option_name)
{
  this->boolean_opt_names[specifier] = option_name;
}

void CmdLineArgsParser::register_integer(const char * specifier, const char * option_name)
{
  this->integer_opt_names[specifier] = option_name;
}

void CmdLineArgsParser::register_string(const char * specifier, const char * option_name)
{
  this->string_opt_names[specifier] = option_name;
}

void CmdLineArgsParser::registerCommaSepIntList(const char * specifier, const char * optionName) {
  myCommaSepIntListOptNames[specifier] = optionName;
}

void CmdLineArgsParser::registerCommaSepList(const char * specifier, const char * optionName) {
  m_CommaSepListOptNames[specifier] = optionName;
}

uint32_t CmdLineArgsParser::parse(Arguments & arguments, const CmdLineArgs & cmdLineArgs)
{
  uint32_t i = 0;
  while (i < cmdLineArgs.getCount())
  {
    const char * arg = cmdLineArgs.get(i).c_str();
    if (is_boolean_spec(arg))
    {
      arguments.put_boolean(this->boolean_opt_names[arg].c_str());
      i++;
      continue;
    }
    if ((i+1) == cmdLineArgs.getCount())
    {
      break;
    }
    const char * optarg = cmdLineArgs.get(i+1).c_str();
    if (is_integer_spec(arg)) {
      arguments.put_integer(this->integer_opt_names[arg].c_str(), std::stoi(optarg));
    }
    else if (is_string_spec(arg)) {
      arguments.put_string(this->string_opt_names[arg].c_str(), optarg);
    }
    else if (isCommaSepListSpec(arg)) {
      set<string> tokens;
      m_ToolBox->parseToTokens(optarg, ",", tokens);
      for (auto & token : tokens) {
        arguments.putToStringList(m_CommaSepListOptNames[arg].c_str(), token);
      }
    }
    else if (isCommaSepIntListSpec(arg)) {
      set<string> tokens;
      m_ToolBox->parseToTokens(optarg, ",", tokens);
      for (auto & token : tokens) {
        arguments.putToIntList(myCommaSepIntListOptNames[arg].c_str(), std::stoi(token.c_str()));
      }
    }
    else {
      break;
    }
    i+=2;
  }
  return i;
}
