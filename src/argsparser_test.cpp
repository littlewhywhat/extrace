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

#include <gtest/gtest.h>

#include "argsparser.h"

//! Tests ArgsParser
class ArgsParserTest : public ::testing::Test {
  public:
    //! Tests parsing of boolean
    void test_parse_boolean() {
      const char * option_name = "option_one";
      const int    argc        = 2;
      const char * argv[]      = { "argsparser_test", "-op" };
      Arguments args;
      parser.register_boolean("-op", option_name);
      parser.parse(args, argc, argv);
      ASSERT_TRUE(args.is_enabled(option_name));
    }
    //! Tests parsing of integer
    void test_parse_integer() {
      // TODO refactor
      const char * option_names[] = { "option_one", "option_two" };
      const char * option_specs[] = { "-op1", "-op2" };
      const int    option_vals[]  = { 4, 44 };
      const int    argc           = 5;
      const char * argv[]         = { "argsparser_test",
                                      option_specs[0], "4",
                                      option_specs[1], "44" };
      Arguments args;
      parser.register_integer(option_specs[0], option_names[0]);
      parser.register_integer(option_specs[1], option_names[1]);
      parser.parse(args, argc, argv);
      ASSERT_TRUE(args.has_integer(option_names[0]));
      ASSERT_EQ(args.get_integer(option_names[0]), option_vals[0]);
      ASSERT_TRUE(args.has_integer(option_names[1]));
      ASSERT_EQ(args.get_integer(option_names[1]), option_vals[1]);
    }
    //! Tests parsing of string
    void test_parse_string() {
      // TODO refactor
      const char * option_names[] = { "option_one", "option_two" };
      const char * option_specs[] = { "-op1", "-op2" };
      const char * option_vals[]  = { "value_one", "value_two"};
      const int    argc           = 5;
      const char * argv[]         = { "argsparser_test",
                                      option_specs[0], option_vals[0],
                                      option_specs[1], option_vals[1] };
      Arguments args;
      parser.register_string(option_specs[0], option_names[0]);
      parser.register_string(option_specs[1], option_names[1]);
      parser.parse(args, argc, argv);
      ASSERT_TRUE(args.has_string(option_names[0]));
      ASSERT_STREQ(args.get_string(option_names[0]).c_str(), option_vals[0]);
      ASSERT_TRUE(args.has_string(option_names[1]));
      ASSERT_STREQ(args.get_string(option_names[1]).c_str(), option_vals[1]);
    }
    //! Tests parsing
    void test_parse() {
      // TODO refactor
      const char * option_names[]   = { "boolean", "integer", "string" };
      const char * option_specs[]   = { "-op_b", "-op_i", "-op_s" };
      const char * option_str_val = "value";
      const int    option_int_val   = 10;
      const int    argc           = 6;
      const char * argv[]         = { "argsparser_test",
                                      option_specs[0],
                                      option_specs[1], "10",
                                      option_specs[2], option_str_val };
      Arguments args;
      parser.register_boolean(option_specs[0], option_names[0]);
      parser.register_integer(option_specs[1], option_names[1]);
      parser.register_string(option_specs[2], option_names[2]);
      parser.parse(args, argc, argv);
      ASSERT_TRUE(args.is_enabled(option_names[0]));
      ASSERT_TRUE(args.has_integer(option_names[1]));
      ASSERT_EQ(args.get_integer(option_names[1]), option_int_val);
      ASSERT_TRUE(args.has_string(option_names[2]));
      ASSERT_STREQ(args.get_string(option_names[2]).c_str(), option_str_val);
    }
    //! Tests invalid parsing
    void test_invalid_parse_boolean() {
      const int    argc           = 2;
      const char * argv[]         = { "argsparser_test",
                                      "-b" };
      Arguments args;
      ASSERT_EQ(parser.parse(args, argc, argv), 1);
    }
    //! Tests invalid parsing
    void test_invalid_parse_int_or_string() {
      const int    argc           = 3;
      const char * argv[]         = { "argsparser_test",
                                      "-b", "unknown" };
      Arguments args;
      ASSERT_EQ(parser.parse(args, argc, argv), 1);
    }

  private:
    //! Tested ArgsParser
    ArgsParser parser;
};

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
}