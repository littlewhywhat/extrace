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

#include "cpurssfilter.h"
#include "cpuguard.h"
#include "rssguard.h"

#include <gtest/gtest.h>

#define CPU_LIMIT 5
#define RSS_LIMIT 10

//! Tests CpuRssFilter
class CpuRssFilterTest : public ::testing::Test
{
  public:
    void SetUp() {
      myCpuRssFilter = new CpuRssFilter(new CpuGuard(CPU_LIMIT),
                                        new RssGuard(RSS_LIMIT));
    }

    void TearDown() {
      delete myCpuRssFilter;
      while (!myRecords.empty()) {
        auto * record = myRecords.front();
        myRecords.pop();
        delete record;
      }
    }

    void testProcess() {
      vector<ProcessRecord*> records = {
        (new ProcessRecord())->setPID(0)->setCpuUsage(CPU_LIMIT),
        // rss is not init, doesn't pass cpu
        (new ProcessRecord())->setPID(1)->setCpuUsage(CPU_LIMIT + 2),
        // rss is not init, does pass cpu
        (new ProcessRecord())->setPID(2)->setRss(5),
        // pass to init rss
        (new ProcessRecord())->setPID(3)->setRss(5 + RSS_LIMIT)->setCpuUsage(CPU_LIMIT),
        // invalid rss and cpu
        (new ProcessRecord())->setPID(4)->setRss(6 + RSS_LIMIT)->setCpuUsage(CPU_LIMIT),
        // pass with valid rss only
        (new ProcessRecord())->setPID(5)->setRss(5 + RSS_LIMIT)->setCpuUsage(CPU_LIMIT + 1),
        // pass with valid cpu only
        (new ProcessRecord())->setPID(6)->setRss(6 + RSS_LIMIT)->setCpuUsage(CPU_LIMIT + 1)
        // pass with both valid
      };
      for (auto * record: records) {
        myRecords.push(record);
      }
      size_t expectedSize = records.size() - 2;
      myCpuRssFilter->process(myRecords);
      EXPECT_EQ(myRecords.size(), expectedSize);

      for (size_t i = 0; i < records.size(); i++) {
        if (i == 0 || i == 3) { // these records doesn't pass
          continue;
        }
        auto * record = myRecords.front();
        myRecords.pop();
        EXPECT_EQ(*(records[i]), *record);
      }
    }

    void testProcessRssWithChange() {
      ProcessRecord * recordOne = (new ProcessRecord())->setRss(11);
      myRecords.push(recordOne);
      myRecords.push((new ProcessRecord())->setRss(20));
      size_t expectedSize = myRecords.size() - 1;
      myCpuRssFilter->process(myRecords);
      EXPECT_EQ(myRecords.size(), expectedSize);
      EXPECT_EQ(myRecords.front(), recordOne);

      myRecords.push((new ProcessRecord())->setRss(21));
      myCpuRssFilter->process(myRecords);
      EXPECT_EQ(myRecords.size(), expectedSize);
      EXPECT_EQ(myRecords.front(), recordOne);      
      
      myRecords.push((new ProcessRecord())->setRss(22));
      myCpuRssFilter->process(myRecords);
      EXPECT_EQ(myRecords.size(), expectedSize + 1);
    }

    void testProcessRssNoChange() {
      myRecords.push((new ProcessRecord())->setRss(11));
      size_t expectedSize = myRecords.size();
      myCpuRssFilter->process(myRecords);
      EXPECT_EQ(myRecords.size(), expectedSize);

      myRecords.push((new ProcessRecord())->setRss(22));
      expectedSize = myRecords.size();
      myCpuRssFilter->process(myRecords);
      EXPECT_EQ(myRecords.size(), expectedSize);
    }

    void testProcessCpuUseWithChange() {
      // define rss border
      auto * recordOne = (new ProcessRecord())->setRss(0);
      myRecords.push(recordOne);
      // all records before 10 should be discarded unless cpu is good

      myRecords.push((new ProcessRecord())->setCpuUsage(5)->setRss(RSS_LIMIT-1));
      size_t expectedSize = myRecords.size() - 1;
      myCpuRssFilter->process(myRecords);
      EXPECT_EQ(myRecords.size(), expectedSize);
      EXPECT_EQ(myRecords.front(), recordOne);

      myRecords.push((new ProcessRecord())->setCpuUsage(6)->setRss(RSS_LIMIT-1));
      expectedSize++;
      myCpuRssFilter->process(myRecords);
      EXPECT_EQ(myRecords.size(), expectedSize);
    }

    void testProcessCpuUseNoChange() {
      myRecords.push((new ProcessRecord())->setCpuUsage(6));
      myRecords.push((new ProcessRecord())->setCpuUsage(7));
      size_t expectedSize = myRecords.size();
      myCpuRssFilter->process(myRecords);
      EXPECT_EQ(myRecords.size(), expectedSize);

      myRecords.push((new ProcessRecord())->setCpuUsage(22));
      expectedSize = myRecords.size();
      myCpuRssFilter->process(myRecords);
      EXPECT_EQ(myRecords.size(), expectedSize);
    }
  private:
    CpuRssFilter * myCpuRssFilter;
    queue<ProcessRecord*> myRecords;
};

TEST_F(CpuRssFilterTest, testProcess) {
  testProcess();
}

TEST_F(CpuRssFilterTest, testProcessRssNoChange) {
  testProcessRssNoChange();
}

TEST_F(CpuRssFilterTest, testProcessRssWithChange) {
  testProcessRssWithChange();
}

TEST_F(CpuRssFilterTest, testProcessCpuUseNoChange) {
  testProcessCpuUseNoChange();
}

TEST_F(CpuRssFilterTest, testProcessCpuUseWithChange) {
  testProcessCpuUseWithChange();
}