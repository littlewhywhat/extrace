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

#include <gtest/gtest.h>

class ArgumentsTest : public ::testing::Test
{
  public:
    //! Tests boolean options
    void test_boolean()
    {
      ASSERT_FALSE(this->args.is_enabled("async_start"));
      ASSERT_FALSE(this->args.is_enabled("sync_start"));
      this->args.put_boolean("async_start");
      this->args.put_boolean("sync_start");
      ASSERT_TRUE(this->args.is_enabled("async_start"));
      ASSERT_TRUE(this->args.is_enabled("sync_start"));
    }
    //! Tests single integer options
    void test_single_integers()
    {
      const int timeout = 15;
      ASSERT_FALSE(this->args.has_integer("timeout"));
      this->args.put_integer("timeout", timeout);
      ASSERT_TRUE(this->args.has_integer("timeout"));
      ASSERT_EQ(this->args.get_integer("timeout"), timeout);
    }
    //! Tests single string options
    void test_single_strings()
    {
      const std::string module = "main";
      ASSERT_FALSE(this->args.has_string("entry"));
      this->args.put_string("entry", module);
      ASSERT_TRUE(this->args.has_string("entry"));
      ASSERT_STREQ(this->args.get_string("entry").c_str(), module.c_str());
    }
  private:
    Arguments args;
};

TEST_F(ArgumentsTest, boolean) {
  test_boolean();
}

TEST_F(ArgumentsTest, integer) {
  test_single_integers();
}

TEST_F(ArgumentsTest, string) {
  test_single_strings();
}