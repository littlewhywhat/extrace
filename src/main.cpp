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

#include "greeter.h"
#include "argsparser.h"
#include "arguments.h"

int main(int argc, const char ** argv)
{
  Arguments args;
  ArgsParser parser;
  parser.register_string("-name", "name");
  int i = parser.parse(args, argc, argv);
  if (i < argc)
  {
    printf("Invalid option: %s\n", argv[i]);
    return 1;
  }

  Greeter greeter;
  std::string whom;
  if (args.has_string("name"))
  {
    whom = args.get_string("name");
  }
  else
  {
    whom = "World";
  }
  greeter.greet(whom.c_str());
  return 0;
}
