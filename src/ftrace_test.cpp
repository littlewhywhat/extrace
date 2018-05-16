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

/*
   This is a template for test of FTrace.
   Things to substitute:
    - FTrace, ftrace
    - FileSystem, filesystem
    - TryEnableTracePoint, tryEnableTracePoint
   After test is run and fails with testThatIrun,
   remove it and uncomment other methods
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ftrace.h"
#include "mock_filesystem.h"
#include "mock_toolbox.h"
#include "wire.h"

#include <memory>

using namespace std;
using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;

//! I am a test of FTrace. I can test its methods.
class FTraceTest : public ::testing::Test {
  public:
    //! Setups my resources
    void SetUp() {
      myMockFileSystem = new MockFileSystem();
      myMockToolBox = new MockToolBox();
      auto sp_myMockFileSystem = shared_ptr<FileSystem>(myMockFileSystem);
      auto sp_myMockToolBox = shared_ptr<ToolBox>(myMockToolBox);
      myWire = new Wire(stdout,stdout);
      myFTrace = new FTrace(*myWire,
                            sp_myMockFileSystem,
                            sp_myMockToolBox,
                            myFTraceMountPoint);
    }

    //! Tears down my resources
    void TearDown() {
      // assumed here that MockFileSystem is managed by FTrace
      delete myFTrace;
      delete myWire;
      myMockFileSystem = NULL;
      myFTrace = NULL;
    }

    //! Tests FTrace's tryEnableTracePoint method with accessable tracepoint
    void testTryEnableAccessableTracePoint(const FTrace::TracePoint & tracePoint,
                                           const char * tracePointPath) {
      EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(tracePointPath), StrEq("1")))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(_))
                                      .WillOnce(Return(true));
      myFTrace->tryEnableTracePoint(tracePoint);
    }

    //! Tests FTrace's tryEnableTracePoint method with not accessable tracepoint
    void testTryEnableNotAccessableTracePoint(const FTrace::TracePoint & tracePoint) {
      EXPECT_CALL(*myMockFileSystem, writeStr(_,_))
                                      .Times(0);
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(_))
                                      .WillOnce(Return(false));
      myFTrace->tryEnableTracePoint(FTrace::TracePoint::SCHED_SWITCH);
    }

  private:
    //! Tested instance of FTrace
    FTrace * myFTrace = NULL;
    MockFileSystem * myMockFileSystem = NULL;
    MockToolBox * myMockToolBox = NULL;
    Wire * myWire = NULL;
    const string myFTraceMountPoint;
};

TEST_F(FTraceTest, testTryEnableAccessableTracePoint)
{
  testTryEnableAccessableTracePoint(FTrace::TracePoint::SCHED_SWITCH,
                                    "/tracing/events/sched/sched_switch/enable");
}

TEST_F(FTraceTest, testTryEnableNotAccessableTracePoint)
{
  testTryEnableNotAccessableTracePoint(FTrace::TracePoint::SCHED_SWITCH);
}
