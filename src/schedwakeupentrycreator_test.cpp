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

#include "schedwakeupentrycreator.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgReferee;
  
//! I am a test. I test my SchedWakeUpEntryCreator
class SchedWakeUpEntryCreatorTest : public ::testing::Test {
  public:
    void testCreateFromCorrectContent() {
      auto * entry = mySchedWakeUpEntryCreator.create(1, 2, 3, 
                                                      "comm=memeater pid=2231 prio=120"
                                                      " success=1 target_cpu=000");
      EXPECT_EQ(entry->getPID(), 1);
      EXPECT_EQ(entry->getName(), string("SchedWakeup"));
      EXPECT_EQ(entry->getTimeLow(), 2);
      EXPECT_EQ(entry->getTimeHigh(), 3);
      EXPECT_EQ(entry->getCommandName(), "memeater");
      EXPECT_EQ(entry->getWakedUpPID(), 2231);
      EXPECT_EQ(entry->getSuccess(), true);
      EXPECT_EQ(entry->getTargetCPU(), 0);
    }
  private:
    //! My SchedWakeUpEntryCreator that I test
    SchedWakeUpEntryCreator mySchedWakeUpEntryCreator;
};

TEST_F(SchedWakeUpEntryCreatorTest, testCreateFromCorrectContent) {
  testCreateFromCorrectContent();
}