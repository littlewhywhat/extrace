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

#include "cpuguard.h"

#include <gtest/gtest.h>

#define LIMIT 5

//! Tests CpuRssFilter
class CpuGuardTest : public ::testing::Test
{
  public:
    void SetUp() {
      myCpuGuard = new CpuGuard(LIMIT);
      myProcessRecord = new ProcessRecord();
    }
    void TearDown() {
      delete myCpuGuard;
      delete myProcessRecord;
    }

    void testPassTrue() {
      myProcessRecord->setCpuUsage(6);
      EXPECT_TRUE(myCpuGuard->pass(*myProcessRecord));
    }

    void testPassFalse() {
      EXPECT_FALSE(myCpuGuard->pass(*myProcessRecord));
      myProcessRecord->setCpuUsage(4);
      EXPECT_FALSE(myCpuGuard->pass(*myProcessRecord));
    }
  private:
    CpuGuard * myCpuGuard;
    ProcessRecord * myProcessRecord;
};

TEST_F(CpuGuardTest, passTrue) {
  testPassTrue();
}

TEST_F(CpuGuardTest, passFalse) {
  testPassFalse();
}