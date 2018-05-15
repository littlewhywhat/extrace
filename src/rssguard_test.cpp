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

#include "rssguard.h"

#include <gtest/gtest.h>

#define LIMIT 5

//! Tests CpuRssFilter
class RssGuardTest : public ::testing::Test
{
  public:
    void SetUp() {
      myRssGuard = new RssGuard(LIMIT);
      myProcessRecord = new ProcessRecord();
    }
    void TearDown() {
      delete myRssGuard;
      delete myProcessRecord;
    }

    void testPassTrue() {
      myProcessRecord->setRss(7);
      EXPECT_TRUE(myRssGuard->pass(*myProcessRecord, myHistory));
      myProcessRecord->setRss(8);
      EXPECT_TRUE(myRssGuard->pass(*myProcessRecord, myHistory));
    }

    void testPassFalse() {
      EXPECT_FALSE(myRssGuard->pass(*myProcessRecord, myHistory));
      myProcessRecord->setRss(0);
      EXPECT_FALSE(myRssGuard->pass(*myProcessRecord, myHistory));
      myProcessRecord->setRss(5);
      EXPECT_FALSE(myRssGuard->pass(*myProcessRecord, myHistory));
      myProcessRecord->setRss(6);
      EXPECT_FALSE(myRssGuard->pass(*myProcessRecord, myHistory));
    }
  private:
    RssGuard * myRssGuard;
    ProcessRecord * myProcessRecord;
    uint64_t myHistory = 1;
};

TEST_F(RssGuardTest, passTrue) {
  testPassTrue();
}

TEST_F(RssGuardTest, passFalse) {
  testPassFalse();
}