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
#include <gmock/gmock.h>

#include "androidtoolbox.h"

using namespace std;
using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::SetArrayArgument;

//! I am a test of AndroidToolBox. I can test its methods.
class AndroidToolBoxTest : public ::testing::Test {
  public:
    //! Setups my resources
    void SetUp() {
      myAndroidToolBox = new AndroidToolBox();
    }

    //! Tears down my resources
    void TearDown() {
      // assumed here that MockDependency is managed by AndroidToolBox
      delete myAndroidToolBox;
      myAndroidToolBox = NULL;
    }

    //! Tests AndroidToolBox's parseToTokens method
    void testParseToTokensComma() {
      const char * delimseplist = "item1,item2,item3";
      const set<string> list = { "item1", "item2", "item3" };
      set<string> tokens;
      myAndroidToolBox->parseToTokens(delimseplist, ",", tokens);
      EXPECT_EQ(tokens, list);
    }

    void testParseToTokensNotOnlyComma() {
      const char * delimseplist = "item1,item2:item3";
      const set<string> list = { "item1", "item2", "item3" };
      set<string> tokens;
      myAndroidToolBox->parseToTokens(delimseplist, ",:", tokens);
      EXPECT_EQ(tokens, list);
    }
  private:
    //! Tested instance of AndroidToolBox
    AndroidToolBox * myAndroidToolBox = NULL;
};

TEST_F(AndroidToolBoxTest, testParseToTokensComma)
{
  testParseToTokensComma();
}

TEST_F(AndroidToolBoxTest, testParseToTokensNotOnlyComma)
{
  testParseToTokensNotOnlyComma();
}