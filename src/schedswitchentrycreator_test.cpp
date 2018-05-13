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

#include "schedswitchentrycreator.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgReferee;
  
//! I am a test. I test my SchedSwitchEntryCreator
class SchedSwitchEntryCreatorTest : public ::testing::Test {
  public:
    void SetUp() {

    }

    void TearDown() {

    }

    void testCreateFromCorrectContent() {
      auto * entry = mySchedSwitchEntryCreator.create(1, 2, 3, "prev_comm=swapper prev_pid=0"
                                                               " prev_prio=120 prev_state=R+ ==>"
                                                               " next_comm=memeater next_pid=2231"
                                                               " next_prio=120");
      EXPECT_EQ(entry->getPID(), 1);
      EXPECT_EQ(entry->getTimeLow(), 2);
      EXPECT_EQ(entry->getTimeHigh(), 3);
      EXPECT_EQ(entry->getPrevCommandName(), "swapper");
      EXPECT_EQ(entry->getPrevPriority(), 120);
      EXPECT_EQ(entry->getPrevPID(), 0);
      EXPECT_EQ(entry->getNextCommandName(), "memeater");
      EXPECT_EQ(entry->getNextPriority(), 120);
      EXPECT_EQ(entry->getNextPID(), 2231);
    }
  private:
    //! My SchedSwitchEntryCreator that I test
    SchedSwitchEntryCreator mySchedSwitchEntryCreator;
};

TEST_F(SchedSwitchEntryCreatorTest, testCreateFromCorrectContent) {
  testCreateFromCorrectContent();
}