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

#include "simpleprocesschangefile.h"
#include "mock_ftraceentryfile.h"
#include "mock_process.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

using namespace std;

using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgReferee;
using ::testing::Sequence;

//! I test SimpleProcessChangeFile.
class SimpleProcessChangeFileTest : public ::testing::Test {
  public:
    void SetUp() {
      myMockProcess = new MockProcess();
      myFTraceEntryFileCreator = new MockFTraceEntryFileCreator();
      myFTraceEntryFile = new MockFTraceEntryFile();
      myProcessChangeFile = new SimpleProcessChangeFile(myFilename,
                                                        myFTraceEntryFileCreator);   
    }

    void TearDown() {
      delete myProcessChangeFile;
      delete myFTraceEntryFile;
      delete myMockProcess;
    }

    void testParseTo() {
      vector<FTraceEntry*> ftraceEntries = {
        (new SchedSwitchEntry(0,1,0))->setPrevPID(0)->setNextPID(1),
        (new TracingMarkEntry(0,2,0))->setTracedPID(1)->setVSS(1)->setUSS(2)->setRSS(3)->setPSS(4),
        (new SchedWakeUpEntry(0,3,0))->setWakedUpPID(0),
        (new SchedSwitchEntry(0,4,0))->setPrevPID(1)->setNextPID(0),
        (new SchedWakeUpEntry(0,5,0))->setWakedUpPID(1),
      };
      EXPECT_CALL(*myFTraceEntryFileCreator, create(StrEq(myFilename)))
                                    .WillOnce(Return(myFTraceEntryFile));
      EXPECT_CALL(*myFTraceEntryFile, parseTo(_))
                                    .WillOnce(SetArgReferee<0>(ftraceEntries));
      Sequence processOne, uss, vss, pss, rss;
      EXPECT_CALL(*myMockProcess, updateTo(1)).Times(1)
                                  .InSequence(processOne);
      EXPECT_CALL(*myMockProcess, setState(ProcessState::RUNNING))
                                  .InSequence(processOne)
                                  .WillOnce(Return(myMockProcess));
      EXPECT_CALL(*myMockProcess, updateTo(2)).Times(1)
                                  .InSequence(processOne, uss, vss, pss, rss);
      EXPECT_CALL(*myMockProcess, setVSS(1))
                                  .InSequence(vss)
                                  .WillOnce(Return(myMockProcess));
      EXPECT_CALL(*myMockProcess, setUSS(2))
                                  .InSequence(uss)
                                  .WillOnce(Return(myMockProcess));
      EXPECT_CALL(*myMockProcess, setRSS(3))
                                  .InSequence(rss)
                                  .WillOnce(Return(myMockProcess));
      EXPECT_CALL(*myMockProcess, setPSS(4))
                                  .InSequence(pss)
                                  .WillOnce(Return(myMockProcess));
      EXPECT_CALL(*myMockProcess, updateTo(4)).Times(1)
                                  .InSequence(processOne, uss, vss, pss, rss);
      EXPECT_CALL(*myMockProcess, setState(ProcessState::SLEEPING))
                                  .InSequence(processOne)
                                  .WillOnce(Return(myMockProcess));
      EXPECT_CALL(*myMockProcess, updateTo(5)).Times(1)
                                  .InSequence(processOne);
      EXPECT_CALL(*myMockProcess, setState(ProcessState::AWAKE))
                                  .InSequence(processOne)
                                  .WillOnce(Return(myMockProcess));

      vector<ProcessChange*> procChanges;
      myProcessChangeFile->parseTo(procChanges);
      EXPECT_EQ(procChanges.size(), 7);
      for (auto * change : procChanges) {
        if (change->getPID() == 1) {
          change->applyTo(*myMockProcess);
        }
      }
    }
      
  private:
    string myFilename = "examplefile";
    ProcessChangeFile * myProcessChangeFile = NULL;
    MockFTraceEntryFile * myFTraceEntryFile = NULL;
    MockFTraceEntryFileCreator * myFTraceEntryFileCreator = NULL;
    MockProcess * myMockProcess = NULL;
};

TEST_F(SimpleProcessChangeFileTest, testParseTo) {
  testParseTo();
}
