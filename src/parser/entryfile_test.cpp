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

#include "parser.h"
#include "simpleentryfile.h"
#include "fakeentrycreator.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <errno.h>  // errno
#include <string.h> // strerror
#include <unistd.h> // access
#include <fcntl.h>  // creat, open

using namespace std;

using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgReferee;
  
//! Tests
class SimpleEntryFileTest : public ::testing::Test {
  public:
    void SetUp() {
      myMockEntryCreator = new MockEntryCreator();
      mySimpleEntryFile = new SimpleEntryFile(myTestFilename, myMockEntryCreator);
    }

    void TearDown() {
      clear();
      delete myMockEntryCreator;
      delete mySimpleEntryFile;
      mySimpleEntryFile = NULL;
    }

    void testParseToMemoryEntry() {
      appendStr("      experiment-2232  ( 2232) [000] ...1  4751.080764:"
                " tracing_mark_write: VSS=102588416  RSS=3674112"
                " PSS=2631237 USS=2592768 PID=2231\n");
      appendStr("          <idle>-0     (-----) [000] dNh4  4751.160159:"
                " sched_wakeup: comm=memeater pid=2231 prio=120"
                " success=1 target_cpu=000\n");
      appendStr("          <idle>-0     (-----) [000] d..3  4751.149653:"
                " sched_switch: prev_comm=swapper prev_pid=0 prev_prio=120 prev_state=R ==>"
                " next_comm=memeater next_pid=2231 next_prio=120\n");
      MemoryEntry * memoryEntry = (new MemoryEntry(2231, 80764, 4751))
                                      ->setVss(102588416)
                                      ->setRss(3674112)
                                      ->setPss(2631237)
                                      ->setUss(2592768);
      EXPECT_CALL(*myMockEntryCreator, create(2232, 80764, 4751, 
                                              StrEq("tracing_mark_write"),
                                              StrEq("VSS=102588416  RSS=3674112"
                                              " PSS=2631237 USS=2592768 PID=2231")))
                  .WillOnce(Return(memoryEntry));
      EXPECT_CALL(*myMockEntryCreator, create(0, 160159, 4751,
                                              StrEq("sched_wakeup"),
                                              StrEq("comm=memeater pid=2231 prio=120"
                                              " success=1 target_cpu=000")))
                  .WillOnce(Return(memoryEntry));
      EXPECT_CALL(*myMockEntryCreator, create(0, 149653, 4751,
                                              StrEq("sched_switch"),
                                              StrEq("prev_comm=swapper prev_pid=0"
                                              " prev_prio=120 prev_state=R ==>"
                                              " next_comm=memeater next_pid=2231 next_prio=120")))
                  .WillOnce(Return(memoryEntry));
      ProcessStat procStatExpected;
      memoryEntry->update(procStatExpected);

      vector<Entry*> entries;
      mySimpleEntryFile->parseTo(entries);
      ASSERT_EQ(entries.size(), 3);

      for (size_t i = 0; i < entries.size(); i++) {
        ProcessStat procStat;
        entries[i]->update(procStat);
        ASSERT_EQ(procStat, procStatExpected);
      }

      //TODO check errors
    }
  private:
    //! Appends string to my test file
    void appendStr(const char * str) {
      int fd = open(myTestFilename.c_str(), O_CREAT|O_APPEND|O_WRONLY);
      if (fd == -1) {
          fprintf(stderr, "error opening %s: %s (%d)\n", myTestFilename.c_str(),
                  strerror(errno), errno);
      }

      ssize_t len = strlen(str);
      if (write(fd, str, len) != len) {
          fprintf(stderr, "error writing to %s: %s (%d)\n", myTestFilename.c_str(),
                  strerror(errno), errno);
      }
      close(fd);
    }

    //! Clears content of my test file 
    void clear() {
      int fd = creat(myTestFilename.c_str(), 0);
      if (fd == -1) {
          fprintf(stderr, "error truncating %s: %s (%d)\n", myTestFilename.c_str(),
                  strerror(errno), errno);
      }
      close(fd);
    }

    //! Path to my test file
    string myTestFilename = "test_example";
    SimpleEntryFile * mySimpleEntryFile = NULL;
    MockEntryCreator * myMockEntryCreator = NULL;
};

TEST_F(SimpleEntryFileTest, testParseToMemoryEntry) {
  testParseToMemoryEntry();
}