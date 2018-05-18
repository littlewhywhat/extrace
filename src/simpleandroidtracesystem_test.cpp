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

    //! Tests SimpleAndroidTraceSystem's tryToTrace method with apps, categories and core
    void testTryToTrace() {
      const string coreservices = "core1,core2,core3";
      EXPECT_CALL(*myMockAndroid, getTraceCoreServicesProperty())
                                      .WillOnce(Return(coreservices));
      set<string> tokens = { "core1", "core2", "core3" };
      EXPECT_CALL(*myMockToolBox, parseToTokens(StrEq(coreservices),
                                                StrEq(","), _))
                  .WillOnce(SetArgReferee<2>(tokens));
      EXPECT_CALL(*myMockAndroid, getTraceAppsMaxNum())
                                      .WillOnce(Return(2+3));
      EXPECT_CALL(*myMockAndroid, trySetTraceAppProperty(StrEq("app1"), 0))
                                      .WillOnce(Return(true));                                      
      EXPECT_CALL(*myMockAndroid, trySetTraceAppProperty(StrEq("app2"), 1))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockAndroid, trySetTraceAppProperty(StrEq("core1"), 2))
                                      .WillOnce(Return(true));                                      
      EXPECT_CALL(*myMockAndroid, trySetTraceAppProperty(StrEq("core2"), 3))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockAndroid, trySetTraceAppProperty(StrEq("core3"), 4))
                                      .WillOnce(Return(true));                                      
      EXPECT_CALL(*myMockAndroid, trySetTraceAppsCntProperty(5))
                                      .WillOnce(Return(true));  
      EXPECT_CALL(*myMockAndroid, tryPokeBinderServices())
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockAndroid, getCategoryTraceTag(Android::TraceCategory::ACTIVITY_MANAGER))
                                      .WillOnce(Return(1));                                      
      EXPECT_CALL(*myMockAndroid, getCategoryTraceTag(Android::TraceCategory::POWER))
                                      .WillOnce(Return(2));
      EXPECT_CALL(*myMockAndroid, trySetTraceTagsProperty(3))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockAndroid, logDumpingTrace())
                                      .Times(1);                          

      const vector<string> apps = { "app1", "app2" };
      const vector<Android::TraceCategory> categories = { Android::TraceCategory::ACTIVITY_MANAGER,
                                                          Android::TraceCategory::POWER };
      mySimpleAndroidTraceSystem->rememberToTraceCoreServices();
      for (auto & app : apps) {
        mySimpleAndroidTraceSystem->rememberToTrace(app);
      }
      for (auto & category : categories) {
        mySimpleAndroidTraceSystem->rememberToTrace(category);
      }
      mySimpleAndroidTraceSystem->tryToTrace();
    }

     void testTryNotToTrace() {
      EXPECT_CALL(*myMockAndroid, getTraceAppsMaxNum())
                                      .WillOnce(Return(6));
      EXPECT_CALL(*myMockAndroid, trySetTraceAppProperty(StrEq(""), 0))
                                      .WillOnce(Return(true));                                      
      EXPECT_CALL(*myMockAndroid, trySetTraceAppProperty(StrEq(""), 1))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockAndroid, trySetTraceAppProperty(StrEq(""), 2))
                                      .WillOnce(Return(true));                                      
      EXPECT_CALL(*myMockAndroid, trySetTraceAppProperty(StrEq(""), 3))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockAndroid, trySetTraceAppProperty(StrEq(""), 4))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockAndroid, trySetTraceAppProperty(StrEq(""), 5))
                                      .WillOnce(Return(true));                                 
      EXPECT_CALL(*myMockAndroid, tryClearTraceAppsCntProperty())
                                      .WillOnce(Return(true));  
      EXPECT_CALL(*myMockAndroid, tryPokeBinderServices())
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockAndroid, trySetTraceTagsProperty(0))
                                      .WillOnce(Return(true));

      mySimpleAndroidTraceSystem->tryNotToTrace();
    }

  private:
    //! Tested instance of SimpleAndroidTraceSystem
    Wire * myWire = NULL;
    MockToolBox * myMockToolBox = NULL;
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

TEST_F(SimpleAndroidTraceSystemTest, testTryToTrace) {
  testTryToTrace();
}

TEST_F(SimpleAndroidTraceSystemTest, testTryNotToTrace) {
  testTryNotToTrace();
}