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

#ifndef LTTLWHWHT_ARGSPARSER_H
#define LTTLWHWHT_ARGSPARSER_H

#include "arguments.h"

//! A parser of command line arguments
class ArgsParser {
  public:
    //! Registers boolean option with specified name under provided specifier
    void register_boolean(const char * specifier, const char * option_name);
    //! Registers integer option with specified name under provided specifier
    void register_integer(const char * specifier, const char * option_name);
    //! Registers string option with specified name under provided specifier
    void register_string(const char * specifier, const char * option_name);
    //! Parses argv using list of registered options to arguments
    void parse(Arguments & arguments, int argc, const char ** argv);
  private:
    //! Stores registered boolean options
    std::map<std::string, std::string> boolean_opt_names;
    //! Stores registered integer options
    std::map<std::string, std::string> integer_opt_names;
    //! Stores registered string options
    std::map<std::string, std::string> string_opt_names;
    //! Checks if specifier is registered in booleans
    bool is_boolean_spec(const char * spec);
    //! Checks if specifier is registered in integers
    bool is_integer_spec(const char * spec);
    //! Checks if specifier is registered in strings
    bool is_string_spec(const char * spec);
};

#endif