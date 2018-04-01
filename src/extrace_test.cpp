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

#include "greeter_test.cpp"
#include "arguments_test.cpp"
#include "argsparser_test.cpp"

TEST_F(GreeterTest, greet) {
  test_greet();
}

TEST_F(ArgumentsTest, boolean) {
  test_boolean();
}

TEST_F(ArgumentsTest, integer) {
  test_single_integers();
}

TEST_F(ArgumentsTest, string) {
  test_single_strings();
}

TEST_F(ArgsParserTest, parse_boolean) {
  test_parse_boolean();
}

TEST_F(ArgsParserTest, parse_integer) {
  test_parse_integer();
}

TEST_F(ArgsParserTest, parse_string) {
  test_parse_string();
}

TEST_F(ArgsParserTest, parse) {
  test_parse();
}

TEST_F(ArgsParserTest, invalid_parse) {
  test_invalid_parse_boolean();
  test_invalid_parse_int_or_string();
  test_invalid_parse_int();
}