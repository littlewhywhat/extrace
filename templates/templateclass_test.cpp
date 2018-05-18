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

/*
   This is a template for test of ClassName.
   Things to substitute:
    - ClassName, classname
    - Dependency, dependency, dependencyMethodName
    - MethodName, methodName, 
   After test is run and fails with testThatIrun,
   remove it and uncomment other methods
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "classname.h"
// #include "mock_dependency.h"

using namespace std;
using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::SetArrayArgument;

//! I am a test of ClassName. I can test its methods.
class ClassNameTest : public ::testing::Test {
  public:
    //! Setups my resources
    // void SetUp() {
    //   myMockDependency = new MockDependency();
    //   myClassName = new ClassName(myMockDependency);
    // }

    //! Tears down my resources
    // void TearDown() {
    //   // assumed here that MockDependency is managed by ClassName
    //   delete myClassName;
    //   myMockDependency = NULL;
    //   myClassName = NULL;
    // }

    //! Tests ClassName's methodName method
    // void testMethodName() {
    //   EXPECT_CALL(*myMockDependency, mockMethod())
    //                                   .WillOnce(Return(true));
    //   myClassName->methodName();
    // }

    void testThatIrun() {
      ASSERT_TRUE(false);
    }
  private:
    //! Tested instance of ClassName
    ClassName * myClassName = NULL;
    // MockDependency * myMockDependency = NULL;

};

// TEST_F(ClassNameTest, testMethodName)
// {
//   testMethodName();
// }

TEST_F(ClassNameTest, testThatIrun)
{
  testThatIrun();
}