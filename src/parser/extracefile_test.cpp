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

#include "entry.h"
#include "processstat.h"
#include "extracefile.h"
#include "mock_entryfile.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgReferee;
  
//! I test ExtraceFile.
class ExtraceFileTest : public ::testing::Test {
  public:
    void SetUp() {
      myMockEntryFile = new MockEntryFile();
      myFile = new ExtraceFile("test/example", new FakeEntryFileCreator(myMockEntryFile));
      myMemoryEntries = 
      {
        (new MemoryEntry(0, 4, 10))->setVss(1)->setRss(2)->setUss(3)->setPss(4),
        (new MemoryEntry(0, 4, 11))->setVss(6)->setRss(7)->setUss(8)->setPss(9),
        (new MemoryEntry(0, 11, 19))->setVss(10)->setRss(11)->setUss(12)->setPss(13),
        (new MemoryEntry(0, 25, 30))->setVss(15)->setRss(16)->setUss(17)->setPss(18)
      };
      myCpuUsageEntries =
      {
        (new CpuUsageEntry(0, 1, 10))->setCpuUsage(1),
        (new CpuUsageEntry(0, 12, 4))->setCpuUsage(6),
        (new CpuUsageEntry(0, 1, 31))->setCpuUsage(10),
        (new CpuUsageEntry(0, 25, 30))->setCpuUsage(15)
      };
    }

    void TearDown() {
      for (auto & entry : myMemoryEntries)
        delete entry;
      delete myMockEntryFile;
      myMockEntryFile = NULL;
      delete myFile;
      myFile = NULL;
    }

    void testParseToOnlyMemoryEntries() {
      for (auto * memoryEntry : myMemoryEntries)
        myMockEntryFileReturn.push_back(memoryEntry);
      EXPECT_CALL(*myMockEntryFile,
                   parseTo(_))
                   .WillOnce(SetArgReferee<0>(myMockEntryFileReturn));
      vector<ProcessStat> procStats;
      ASSERT_TRUE(myFile->tryParseTo(procStats));
      ASSERT_EQ(procStats.size(), 0);
    }

    void testParseOneMemoryOneCpuUsage() {
      auto & memoryEntry = *myMemoryEntries[0];
      auto & cpuUsageEntry = *myCpuUsageEntries[0];
      int pid = 5;
      memoryEntry.setPID(pid);
      cpuUsageEntry.setPID(pid);
      ProcessStat * processStat = (new ProcessStat())
                                  ->setPID(pid)
                                  ->setTimeLow(cpuUsageEntry.getTimeLow())
                                  ->setTimeHigh(cpuUsageEntry.getTimeHigh())
                                  ->setVss(memoryEntry.getVss())
                                  ->setPss(memoryEntry.getPss())
                                  ->setUss(memoryEntry.getUss())
                                  ->setRss(memoryEntry.getRss())
                                  ->setCpuUsage(cpuUsageEntry.getCpuUsage());
      myMockEntryFileReturn.push_back(&memoryEntry);
      myMockEntryFileReturn.push_back(&cpuUsageEntry);
      EXPECT_CALL(*myMockEntryFile,
                   parseTo(_))
                   .WillOnce(SetArgReferee<0>(myMockEntryFileReturn));
      vector<ProcessStat> procStats;
      ASSERT_TRUE(myFile->tryParseTo(procStats));
      ASSERT_EQ(procStats.size(), 1);
      ASSERT_EQ(procStats.front(), *processStat);
      delete processStat;
    }
    void testParseMultiplePIDs() {
      auto & memoryEntryPID1 = *myMemoryEntries[0];
      auto & cpuUsageEntryPID1 = *myCpuUsageEntries[0];
      auto & memoryEntryPID2 = *myMemoryEntries[1];
      auto & cpuUsageEntryPID2 = *myCpuUsageEntries[1];
      myMockEntryFileReturn.push_back(memoryEntryPID1.setPID(1));
      myMockEntryFileReturn.push_back(cpuUsageEntryPID2.setPID(2));
      myMockEntryFileReturn.push_back(memoryEntryPID2.setPID(2));
      myMockEntryFileReturn.push_back(cpuUsageEntryPID1.setPID(1));
      EXPECT_CALL(*myMockEntryFile,
                   parseTo(_))
                   .WillOnce(SetArgReferee<0>(myMockEntryFileReturn));
      ProcessStat * processStatPID1 = (new ProcessStat())
                                  ->setPID(1)
                                  ->setTimeLow(cpuUsageEntryPID1.getTimeLow())
                                  ->setTimeHigh(cpuUsageEntryPID1.getTimeHigh())
                                  ->setVss(memoryEntryPID1.getVss())
                                  ->setPss(memoryEntryPID1.getPss())
                                  ->setUss(memoryEntryPID1.getUss())
                                  ->setRss(memoryEntryPID1.getRss())
                                  ->setCpuUsage(cpuUsageEntryPID1.getCpuUsage());
      ProcessStat * processStatPID2 = (new ProcessStat())
                                  ->setPID(2)
                                  ->setTimeLow(memoryEntryPID2.getTimeLow())
                                  ->setTimeHigh(memoryEntryPID2.getTimeHigh())
                                  ->setVss(memoryEntryPID2.getVss())
                                  ->setPss(memoryEntryPID2.getPss())
                                  ->setUss(memoryEntryPID2.getUss())
                                  ->setRss(memoryEntryPID2.getRss())
                                  ->setCpuUsage(cpuUsageEntryPID2.getCpuUsage());
      vector<ProcessStat> procStats;
      ASSERT_TRUE(myFile->tryParseTo(procStats));
      ASSERT_EQ(procStats.size(), 2);
      ASSERT_EQ(procStats[0], *processStatPID2);
      ASSERT_EQ(procStats[1], *processStatPID1);
    }
  private:
    ExtraceFile * myFile = NULL;
    MockEntryFile * myMockEntryFile = NULL;
    vector<MemoryEntry*> myMemoryEntries;
    vector<CpuUsageEntry*> myCpuUsageEntries;
    vector<Entry*> myMockEntryFileReturn;
};

TEST_F(ExtraceFileTest, testParseOneMemoryOneCpuUsage) {
  testParseOneMemoryOneCpuUsage();
}

TEST_F(ExtraceFileTest, parseToOnlyMemoryEntries) {
  testParseToOnlyMemoryEntries();
}

TEST_F(ExtraceFileTest, testParseMultiplePIDs) {
  testParseMultiplePIDs();
}