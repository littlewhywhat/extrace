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
   This is a template for test of SimpleAndroidTraceSystem.
   Things to substitute:
    - SimpleAndroidTraceSystem, simpleandroidtracesystem
    - Android, android
    - CanTraceCoreServices, canTraceCoreServices
   After test is run and fails with testThatIrun,
   remove it and uncomment other methods
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "simpleandroidtracesystem.h"
// #include "mock_android.h"

using ::testing::Return;

//! I am a test of SimpleAndroidTraceSystem. I can test its methods.
class SimpleAndroidTraceSystemTest : public ::testing::Test {
  public:
    //! Setups my resources
    // void SetUp() {
    //   myMockAndroid = new MockAndroid();
    //   mySimpleAndroidTraceSystem = new SimpleAndroidTraceSystem(myMockAndroid);
    // }

    //! Tears down my resources
    // void TearDown() {
    //   // assumed here that MockAndroid is managed by SimpleAndroidTraceSystem
    //   delete mySimpleAndroidTraceSystem;
    //   myMockAndroid = NULL;
    //   mySimpleAndroidTraceSystem = NULL;
    // }

    //! Tests SimpleAndroidTraceSystem's canTraceCoreServices method
    // void testCanTraceCoreServices() {
    //   EXPECT_CALL(*myMockAndroid, mockMethod())
    //                                   .AtLeast(1);
    //   mySimpleAndroidTraceSystem->canTraceCoreServices();
    // }

    void testThatIrun() {
      ASSERT_TRUE(false);
    }
  private:
    //! Tested instance of SimpleAndroidTraceSystem
    SimpleAndroidTraceSystem * mySimpleAndroidTraceSystem = NULL;
    // MockAndroid * myMockAndroid = NULL;

};

// TEST_F(SimpleAndroidTraceSystemTest, testCanTraceCoreServices)
// {
//   testCanTraceCoreServices();
// }

TEST_F(SimpleAndroidTraceSystemTest, testThatIrun)
{
  testThatIrun();
}