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
#include "mock_android.h"
#include "mock_toolbox.h"
#include "wire.h"

using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::SetArrayArgument;

//! I am a test of SimpleAndroidTraceSystem. I can test its methods.
class SimpleAndroidTraceSystemTest : public ::testing::Test {
  public:
    //! Setups my resources
    void SetUp() {
      myWire = new Wire(stdout,stdout);
      myMockAndroid = new MockAndroid();
      myMockToolBox = new MockToolBox();
      auto sp_myMockAndroid = shared_ptr<Android>(myMockAndroid);
      auto sp_myMockToolBox = shared_ptr<ToolBox>(myMockToolBox);
      mySimpleAndroidTraceSystem = new SimpleAndroidTraceSystem(*myWire,
                                                                sp_myMockAndroid,
                                                                sp_myMockToolBox);
    }

    //! Tears down my resources
    void TearDown() {
      // assumed here that MockAndroid is managed by SimpleAndroidTraceSystem
      delete mySimpleAndroidTraceSystem;
      delete myWire;
      myMockAndroid = NULL;
      mySimpleAndroidTraceSystem = NULL;
    }

    //! Tests SimpleAndroidTraceSystem's canTraceCoreServices method with no coreservices
    void testCanTraceCoreServicesFalse() {
      EXPECT_CALL(*myMockAndroid, getTraceCoreServicesProperty())
                                      .WillOnce(Return(""));
      EXPECT_FALSE(mySimpleAndroidTraceSystem->canTraceCoreServices());
    }

    //! Tests SimpleAndroidTraceSystem's canTraceCoreServices method with coreservices
    void testCanTraceCoreServicesTrue() {
      EXPECT_CALL(*myMockAndroid, getTraceCoreServicesProperty())
                                      .WillOnce(Return("some_service"));
      EXPECT_TRUE(mySimpleAndroidTraceSystem->canTraceCoreServices());
    }

  private:
    //! Tested instance of SimpleAndroidTraceSystem
    Wire * myWire = NULL;
    ToolBox * myMockToolBox = NULL;
    MockAndroid * myMockAndroid = NULL;
    SimpleAndroidTraceSystem * mySimpleAndroidTraceSystem = NULL;
};

TEST_F(SimpleAndroidTraceSystemTest, testCanTraceCoreServicesTrue)
{
  testCanTraceCoreServicesTrue();
}

TEST_F(SimpleAndroidTraceSystemTest, testCanTraceCoreServicesFalse)
{
  testCanTraceCoreServicesFalse();
}

