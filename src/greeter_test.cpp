/*
 * Copyright (C) 2017 Roman Vaivod
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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "greeter.h"

using ::testing::internal::CaptureStdout;
using ::testing::internal::GetCapturedStdout;

class GreeterTest : public ::testing::Test {
  public:
  	void test_greet() {
  	  const char * greeting = "Hello, test!\n";
  	  const char * whom   = "test";
      CaptureStdout();
      this->greeter.greet(whom);
      std::string stdout_ = GetCapturedStdout();
      ASSERT_STREQ(stdout_.c_str(), greeting);
  	}
  private:
  	Greeter greeter;
};

TEST_F(GreeterTest, greet) {
  test_greet();
}
