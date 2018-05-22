/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include "processrecordfile.h"
#include "mock_processchangefile.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgReferee;
  
//! Tests
class SimpleProcessRecordFileTest : public ::testing::Test {
  public:
    void SetUp() {
      myProcessChangeFileCreator = new MockProcessChangeFileCreator();
      myProcessChangeFile = new MockProcessChangeFile();
      myProcessRecordFile = new SimpleProcessRecordFile(myFilename,
                                                        myProcessChangeFileCreator);   
    }

    void TearDown() {
      delete myProcessRecordFile;
      delete myProcessChangeFile;
    }

    void testParseToWithNotifications() {
      vector<ProcessChange*> processChanges = {
        (new StateChange(0,0))->setState(ProcessState::RUNNING),
        (new StateChange(1,0))->setState(ProcessState::SLEEPING),
        (new NotificationChange(0,0))->setCause("Test started"),
        (new NotificationChange(1,0))->setCause("Test started"),
        (new NotificationChange(0,1))->setCause("Test stopped"),
        (new NotificationChange(1,1))->setCause("Test stopped"),
      };
      vector<ProcessRecord*> procRecordsExpect = {
        (new ProcessRecord())->setPID(0)->setTimeStamp(0)->setState(ProcessState::RUNNING),
        (new ProcessRecord())->setPID(1)->setTimeStamp(0)->setState(ProcessState::SLEEPING),
        (new ProcessRecord())->setPID(0)->setTimeStamp(0)->setCause("Test started")
                             ->setState(ProcessState::RUNNING),
        (new ProcessRecord())->setPID(1)->setTimeStamp(0)->setCause("Test started")
                             ->setState(ProcessState::SLEEPING),
        (new ProcessRecord())->setPID(0)->setTimeStamp(1)->setCause("Test stopped")
                             ->setCpuUsage(100)->setState(ProcessState::RUNNING),
        (new ProcessRecord())->setPID(1)->setTimeStamp(1)->setCause("Test stopped")
                             ->setCpuUsage(0)->setState(ProcessState::SLEEPING),
      };
      // TODO better to switch to long long
      EXPECT_CALL(*myProcessChangeFileCreator, create(StrEq(myFilename)))
                                    .WillOnce(Return(myProcessChangeFile));
      EXPECT_CALL(*myProcessChangeFile, parseTo(_))
                                    .WillOnce(SetArgReferee<0>(processChanges));

      vector<ProcessRecord*> procRecords;
      myProcessRecordFile->parseTo(procRecords);
      EXPECT_EQ(procRecords.size(), procRecordsExpect.size());
      for (size_t i = 0; i < procRecords.size(); i++) {
        EXPECT_EQ(*(procRecords[i]), *(procRecordsExpect[i]));
      }
    }

    void testParseTo() {
      vector<ProcessChange*> processChanges = {
        (new StateChange(0,0))->setState(ProcessState::RUNNING),
        (new StateChange(1,0))->setState(ProcessState::SLEEPING),
        (new MemoryChange(0,2))->setVSS(1)->setUSS(1)->setRSS(1)->setPSS(1),
        (new MemoryChange(0,3))->setVSS(2)->setUSS(2)->setRSS(2)->setPSS(2),
        (new StateChange(1,4))->setState(ProcessState::RUNNING),
        (new StateChange(0,4))->setState(ProcessState::SLEEPING),
        (new MemoryChange(1,5))->setVSS(1)->setUSS(1)->setRSS(1)->setPSS(1),
        (new MemoryChange(1,6))->setVSS(2)->setUSS(2)->setRSS(2)->setPSS(2),
      };
      vector<ProcessRecord*> procRecordsExpect = {
        (new ProcessRecord())->setPID(0)->setTimeStamp(0)->setState(ProcessState::RUNNING),
        (new ProcessRecord())->setPID(1)->setTimeStamp(0)->setState(ProcessState::SLEEPING),
        (new ProcessRecord())->setPID(0)->setTimeStamp(2)->setState(ProcessState::RUNNING)
                             ->setVss(1)->setUss(1)->setRss(1)->setPss(1)
                             ->setCpuUsage(100),
        (new ProcessRecord())->setPID(0)->setTimeStamp(3)->setState(ProcessState::RUNNING)
                             ->setVss(2)->setUss(2)->setRss(2)->setPss(2)
                             ->setCpuUsage(100),
        (new ProcessRecord())->setPID(1)->setTimeStamp(4)->setState(ProcessState::RUNNING)
                             ->setCpuUsage(0),
        (new ProcessRecord())->setPID(0)->setTimeStamp(4)->setState(ProcessState::SLEEPING)
                             ->setVss(2)->setUss(2)->setRss(2)->setPss(2)
                             ->setCpuUsage(100),
        (new ProcessRecord())->setPID(1)->setTimeStamp(5)->setState(ProcessState::RUNNING)
                             ->setVss(1)->setUss(1)->setRss(1)->setPss(1)
                             ->setCpuUsage(20),
        (new ProcessRecord())->setPID(1)->setTimeStamp(6)->setState(ProcessState::RUNNING)
                             ->setVss(2)->setUss(2)->setRss(2)->setPss(2)
                             ->setCpuUsage(33),
      };
      // TODO better to switch to long long
      EXPECT_CALL(*myProcessChangeFileCreator, create(StrEq(myFilename)))
                                    .WillOnce(Return(myProcessChangeFile));
      EXPECT_CALL(*myProcessChangeFile, parseTo(_))
                                    .WillOnce(SetArgReferee<0>(processChanges));

      vector<ProcessRecord*> procRecords;
      myProcessRecordFile->parseTo(procRecords);
      EXPECT_EQ(procRecords.size(), procRecordsExpect.size());
      for (size_t i = 0; i < procRecords.size(); i++) {
        EXPECT_EQ(*(procRecords[i]), *(procRecordsExpect[i]));
      }
    }
      
  private:
    string myFilename = "examplefile";
    ProcessRecordFile * myProcessRecordFile = NULL;
    MockProcessChangeFile * myProcessChangeFile = NULL;
    MockProcessChangeFileCreator * myProcessChangeFileCreator = NULL;
};

TEST_F(SimpleProcessRecordFileTest, testParseTo) {
  testParseTo();
}

TEST_F(SimpleProcessRecordFileTest, testParseToWithNotifications) {
  testParseToWithNotifications();
}