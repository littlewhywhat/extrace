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

#include "memoryentrycreator.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgReferee;
  
//! I am a test. I test my MemoryEntryCreator
class MemoryEntryCreatorTest : public ::testing::Test {
  public:
    void SetUp() {

    }

    void TearDown() {

    }

    void testCreateFromCorrectContent() {
      auto * entry = myMemoryEntryCreator.create(1, 2, 3, 
                                  "VSS=102588416  RSS=3674112"
                                  " PSS=2631237 USS=2592768 PID=2231");
      EXPECT_EQ(entry->getPID(), 1);
      EXPECT_EQ(entry->getName(), string("MemTrace"));
      EXPECT_EQ(entry->getTimeLow(), 2);
      EXPECT_EQ(entry->getTimeHigh(), 3);
      EXPECT_EQ(entry->getVSS(), 102588416);
      EXPECT_EQ(entry->getRSS(), 3674112);
      EXPECT_EQ(entry->getPSS(), 2631237);
      EXPECT_EQ(entry->getUSS(), 2592768);
      EXPECT_EQ(entry->getTracedPID(), 2231);
    }
  private:
    //! My MemoryEntryCreator that I test
    MemoryEntryCreator myMemoryEntryCreator;
};

TEST_F(MemoryEntryCreatorTest, testCreateFromCorrectContent) {
  testCreateFromCorrectContent();
}