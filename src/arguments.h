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

#ifndef LTTLWHWHT_ARGUMENTS_H
#define LTTLWHWHT_ARGUMENTS_H

#include <set>
#include <string>
#include <map>
#include <vector>

using namespace std;

//! This class contains arguments' values
class Arguments {
  public:
    //! Enables boolean option specified by option_name
    void put_boolean(const char * option_name);
    //! Says if boolean option specified by option_name is enabled
    bool is_enabled(const char * option_name) const;
    //! Specifies new single integer option with provided option_name
    void put_integer(const char * option_name, int value);
    //! Says if single integer option specified by option_name is present
    bool has_integer(const char * option_name) const;
    //! Returns value for single integer option specified by option name
    int get_integer(const char * option_name) const;
    //! Specifies new single string option with provided option_name
    void put_string(const char * option_name, const std::string & value);
    //! Says if single string option specified by option_name is present
    bool has_string(const char * option_name) const;
    //! Returns value for single string option specified by option name
    const std::string & get_string(const char * option_name) const;
    //! Add new value to list of strings specified by option name
    void putToStringList(const char * optionName, const string & value);
    //! Returns vector of values for list of string option specified by option name
    const vector<string> & getStringList(const char * optionName) const;
    //! Says if list of string option specified by option name exists
    bool hasStringList(const char * optionName) const;
  private:
    //! set of enabled boolean options
    std::set<std::string> booleans;
    //! map of single integer options
    std::map<std::string, int> single_integers;
    //! map of single string options
    std::map<std::string, std::string> single_strings;
    //! map of string list options
    map<string, vector<string>> m_ListStrings;   
};

#endif