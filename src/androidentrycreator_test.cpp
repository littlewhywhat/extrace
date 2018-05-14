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

#include "androidentrycreator.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgReferee;
  
//! I am a test. I test my AndroidEntryCreator
class AndroidEntryCreatorTest : public ::testing::Test {
  public:

    void testCreateFromCorrectContent() {
      const char * content = "B|1311|com.android.server.wm.WindowManagerService$H: #9";
      auto * entry = myAndroidEntryCreator.create(1, 2, 3, content);
      EXPECT_EQ(entry->getPID(), 1);
      EXPECT_EQ(entry->getName(), string(content+2));
      EXPECT_EQ(entry->getTimeLow(), 2);
      EXPECT_EQ(entry->getTimeHigh(), 3);
    }
  private:
    //! My AndroidEntryCreator that I test
    AndroidEntryCreator myAndroidEntryCreator;
};

TEST_F(AndroidEntryCreatorTest, testCreateFromCorrectContent) {
  testCreateFromCorrectContent();
}