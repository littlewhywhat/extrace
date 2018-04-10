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

#include "templateclass.h"
#include "mock_templateinterface.h"

using ::testing::Return;

//! Tests TemplateClassTest
class TemplateClassTest : public ::testing::Test {
  public:
    //! tests method of TemplateClass
    void test_method() {
      MockTemplateInterface * templateinterface = new MockTemplateInterface();
      EXPECT_CALL(*templateinterface, mock_method())
                                      .AtLeast(1);
      templateclass.set_templateinterface(templateinterface);
    }
  private:
    //! Tested instance of TemplateClass
    TemplateClass templateclass;
};

TEST_F(TemplateClassTest, method)
{
  test_method();
}