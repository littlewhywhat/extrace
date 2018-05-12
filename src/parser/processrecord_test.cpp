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

#include "processrecord.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>

using namespace std;

using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgReferee;
using ::testing::internal::CaptureStdout;
using ::testing::internal::GetCapturedStdout;

//! I am a test. I test my SchedSwitchEntryCreator
class ProcessRecordTest : public ::testing::Test {
  public:
    void SetUp() {

    }

    void TearDown() {

    }

    void testPrint() {
      CaptureStdout();
      cout << myProcessRecord;
      string out = GetCapturedStdout();
      EXPECT_EQ(out, string("    ? |   ? % |          ? |          ? |          ? |          ? |           ? | ?"));
      myProcessRecord.setPID(99999)->setCpuUsage(100)
                      ->setVss(1234567890)->setUss(1234567890)->setRss(1234567890)->setPss(1234567890)
                      ->setTimeStamp(12345678901)->setState(ProcessState::RUNNING);
      CaptureStdout();
      cout << myProcessRecord;
      out = GetCapturedStdout();
      EXPECT_EQ(out, string("99999 | 100 % | 1234567890 | 1234567890 | 1234567890 | 1234567890 | 12345678901 | RUNNING"));
    }
  private:
    ProcessRecord myProcessRecord;
};

TEST_F(ProcessRecordTest, testPrint) {
  testPrint();
}