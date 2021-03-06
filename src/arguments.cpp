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

#include "arguments.h"

void Arguments::put_boolean(const char * option_name)
{
  auto & booleans = this->booleans;
  if (booleans.find(option_name) == booleans.end())
  {
    booleans.insert(option_name);
  }
}

bool Arguments::is_enabled(const char * option_name) const
{
  return this->booleans.find(option_name) != booleans.end();
}

void Arguments::put_integer(const char * option_name, int value)
{
  auto & single_integers = this->single_integers;
  if (single_integers.find(option_name) == single_integers.end())
  {
    single_integers.insert(std::make_pair(option_name, value));
  }
}

bool Arguments::has_integer(const char * option_name) const
{
  return single_integers.find(option_name) != single_integers.end();
}

int Arguments::get_integer(const char * option_name) const
{
  return (*single_integers.find(option_name)).second;
}

void Arguments::put_string(const char * option_name, const std::string & value)
{
  auto & single_strings = this->single_strings;
  if (single_strings.find(option_name) == single_strings.end())
  {
    single_strings.insert(std::make_pair(option_name, value));
  }
}

bool Arguments::has_string(const char * option_name) const
{
  return single_strings.find(option_name) != single_strings.end();
}

const std::string & Arguments::get_string(const char * option_name) const
{
  return (*single_strings.find(option_name)).second;
}

void Arguments::putToStringList(const char * optionName, const string & value) {
  if (m_ListStrings.find(optionName) == m_ListStrings.end()) {
    m_ListStrings.insert(std::make_pair(optionName, vector<string>()));
  }
  m_ListStrings[optionName].push_back(value);
}

const vector<string> & Arguments::getStringList(const char * optionName) const {
  return m_ListStrings.at(optionName);
}

bool Arguments::hasStringList(const char * optionName) const {
  return m_ListStrings.find(optionName) != m_ListStrings.end();
}

void Arguments::putToIntList(const char * optionName, int value) {
  if (myListInts.find(optionName) == myListInts.end()) {
    myListInts.insert(std::make_pair(optionName, vector<int>()));
  }
  myListInts[optionName].push_back(value);
}

const vector<int> & Arguments::getIntList(const char * optionName) const {
  return myListInts.at(optionName);
}

bool Arguments::hasIntList(const char * optionName) const {
  return myListInts.find(optionName) != myListInts.end();
}
