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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "simpleftrace.h"
#include "mock_filesystem.h"
#include "mock_toolbox.h"
#include "wire.h"

#include <memory>

using namespace std;
using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::SetArrayArgument;

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
      EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(myFTraceMountPoint
                                                    + tracePointPath), StrEq("1")))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(_))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(myFTrace->tryEnableTracePoint(tracePoint));
    }

    //! Tests FTrace's tryEnableTracePoint method with not accessable tracepoint
    void testTryEnableNotAccessableTracePoint(const FTrace::TracePoint & tracePoint) {
      EXPECT_CALL(*myMockFileSystem, writeStr(_,_))
                                      .Times(0);
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(_))
                                      .WillOnce(Return(false));
      EXPECT_FALSE(myFTrace->tryEnableTracePoint(tracePoint));
    }

    //! Tests FTrace's tryCleanTrace method
    void testTryCleanTrace() {
      EXPECT_CALL(*myMockFileSystem, truncateFile(StrEq(myFTraceMountPoint
                                                        + "/tracing/trace")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(myFTrace->tryCleanTrace());
    }
    //! Tests FTrace's tracePointAccessable method
    void testTracePointAccessable(const FTrace::TracePoint & tracePoint,
                                  const char * tracePointPath) {
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(StrEq(myFTraceMountPoint
                                                          + tracePointPath)))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(myFTrace->tracePointAccessable(tracePoint));
    }
    //! Tests FTrace's tracePointExists method
    void testTracePointExists(const FTrace::TracePoint & tracePoint,
                              const char * tracePointPath) {
      EXPECT_CALL(*myMockFileSystem, fileExists(StrEq(myFTraceMountPoint
                                                      + tracePointPath)))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(myFTrace->tracePointExists(tracePoint));
    }
    //! Tests FTrace's tryWriteMarker method
    void testTryWriteMarker(const char * marker) {
       EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(myFTraceMountPoint
                                                     + "/tracing/trace_marker"),
                                               StrEq(marker)))
                                      .WillOnce(Return(true));
       EXPECT_TRUE(myFTrace->tryWriteMarker(marker));
    }
    //! Tests FTrace's trySetTracer method
    void testTrySetTracer(const FTrace::Tracer & tracer, const char * tracerName) {
       EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(myFTraceMountPoint
                                                     + "/tracing/current_tracer"),
                                               StrEq(tracerName)))
                                      .WillOnce(Return(true));
       EXPECT_TRUE(myFTrace->trySetTracer(tracer));
    }

    //! Tests FTrace's tryClearFunctionFilter method
    void testTryClearFunctionFilter() {
      EXPECT_CALL(*myMockFileSystem, truncateFile(StrEq(myFTraceMountPoint
                                                     + "/tracing/set_ftrace_filter")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(myFTrace->tryClearFunctionFilter());
    }
    //! Tests FTrace's tryAddFunctionToFilter method
    void testTryAddFunctionToFilter(const char * function) {
       EXPECT_CALL(*myMockFileSystem, appendStr(StrEq(myFTraceMountPoint
                                                     + "/tracing/set_ftrace_filter"),
                                                StrEq(function)))
                                      .WillOnce(Return(true));
       EXPECT_TRUE(myFTrace->tryAddFunctionToFilter(function));
    }
    //! Tests FTrace's tracerChoiceAccessable method
    void testTracerChoiceAccessable() {
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(StrEq(myFTraceMountPoint
                                                     + "/tracing/current_tracer")))
                                     .WillOnce(Return(true));
      EXPECT_TRUE(myFTrace->tracerChoiceAccessable());
    }
    //! Tests FTrace's tunctionFilterAccessible method
    void testFunctionFilterAccessible() {
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(StrEq(myFTraceMountPoint
                                                     + "/tracing/set_ftrace_filter")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(myFTrace->functionFilterAccessible());
    }

    //! Tests FTrace's tryStart
    void testTryStartTrace() {
      EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(myFTraceMountPoint
                                                    + "/tracing/tracing_on"),
                                              StrEq("1")))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(StrEq(myFTraceMountPoint
                                                    + "/tracing/tracing_on")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(myFTrace->tryStartTrace());
    }

    //! Tests FTrace's tryStop
    void testTryStopTrace() {
      EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(myFTraceMountPoint
                                                    + "/tracing/tracing_on"),
                                              StrEq("0")))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(StrEq(myFTraceMountPoint
                                                    + "/tracing/tracing_on")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(myFTrace->tryStopTrace());
    }

    //! Tests FTrace's tryGetFunctionsFromFilter
    void testTryGetFunctionsFromFilter() {
      const char * functionFilterContent = "function1\nwild*card\nfunction2\n";
      EXPECT_CALL(*myMockFileSystem, readStr(StrEq(myFTraceMountPoint
                                                     + "/tracing/set_ftrace_filter"),
                                             _, 4097))
                                      .WillOnce(
                                        DoAll(
                                          SetArrayArgument<1>(functionFilterContent,
                                                              functionFilterContent
                                                              + strlen(functionFilterContent)), 
                                                      Return(true)));
      set<string> tokens = { "function1", "wild*card", "function2" };
      EXPECT_CALL(*myMockToolBox, parseToTokens(StrEq(functionFilterContent),
                                                StrEq("\n"), _))
                  .WillOnce(SetArgReferee<2>(tokens));
      const set<string> functions = myFTrace->tryGetFunctionsFromFilter();
      tokens.erase("wild*card");
      EXPECT_EQ(functions, tokens);
    }

    //! Tests FTrace's trySetBufferSize
    void testTrySetBufferSize() {
      EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(myFTraceMountPoint
                                                    + "/tracing/buffer_size_kb"),
                                              StrEq("4294967295")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(myFTrace->trySetBufferSize(UINT32_MAX));
      EXPECT_FALSE(myFTrace->trySetBufferSize(0));
    }

    //! Tests FTrace's tryEnableOption nethod with accessible option
    void testTryEnableAccessableOption(const FTrace::Option & option,
                                       const char * optionPath) {
      EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(myFTraceMountPoint
                                                    + optionPath), StrEq("1")))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(_))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(myFTrace->tryEnableOption(option));
    }

    //! Tests FTrace's tryEnableOption method with not accessable tracepoint
    void testTryEnableNotAccessableOption(const FTrace::Option & option) {
      EXPECT_CALL(*myMockFileSystem, writeStr(_,_))
                                      .Times(0);
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(_))
                                      .WillOnce(Return(false));
      EXPECT_FALSE(myFTrace->tryEnableOption(option));
    }

    //! Tests FTrace's hasTraceClockSetTo
    void testHasTraceClockSetTo() {
      const char * traceClockContent = "global [local]";
      EXPECT_CALL(*myMockFileSystem, readStr(StrEq(myFTraceMountPoint
                                                     + "/tracing/trace_clock"),
                                             _, 4097))
                                      .WillOnce(
                                        DoAll(
                                          SetArrayArgument<1>(traceClockContent,
                                                              traceClockContent
                                                              + strlen(traceClockContent)), 
                                                      Return(true)))
                                      .WillOnce(
                                        DoAll(
                                          SetArrayArgument<1>(traceClockContent,
                                                              traceClockContent
                                                              + strlen(traceClockContent)), 
                                                      Return(true)));
      EXPECT_FALSE(myFTrace->hasTraceClockSetTo(FTrace::ClockType::GLOBAL));
      EXPECT_TRUE(myFTrace->hasTraceClockSetTo(FTrace::ClockType::LOCAL));
    }
  private:
    //! Tested instance of FTrace
    FTrace * myFTrace = NULL;
    MockFileSystem * myMockFileSystem = NULL;
    MockToolBox * myMockToolBox = NULL;
    Wire * myWire = NULL;
    const string myFTraceMountPoint = "/sys/debug";
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

TEST_F(FTraceTest, testTryEnableAccessableOption)
{
  testTryEnableAccessableOption(FTrace::Option::OVERWRITE,
                                "/tracing/options/overwrite");
}

TEST_F(FTraceTest, testTryEnableNotAccessableOption)
{
  testTryEnableNotAccessableOption(FTrace::Option::OVERWRITE);
}

TEST_F(FTraceTest, testTryCleanTrace) {
  testTryCleanTrace();
}

TEST_F(FTraceTest, testTracePointAccessable) {
  testTracePointAccessable(FTrace::TracePoint::SCHED_SWITCH,
                           "/tracing/events/sched/sched_switch/enable");
}

TEST_F(FTraceTest, testTracePointExists) {
  testTracePointExists(FTrace::TracePoint::SCHED_SWITCH,
                       "/tracing/events/sched/sched_switch/enable");
}

TEST_F(FTraceTest, testTryWriteMarker) {
  testTryWriteMarker("marker");
}

TEST_F(FTraceTest, testTrySetTracer) {
  testTrySetTracer(FTrace::Tracer::NOP, "nop");
}

TEST_F(FTraceTest, testTryClearFunctionFilter) {
  testTryClearFunctionFilter();
}

TEST_F(FTraceTest, testTryAddFunctionToFilter) {
  testTryAddFunctionToFilter("function_one");
}

TEST_F(FTraceTest, testTracerChoiceAccessable) {
  testTracerChoiceAccessable();
}

TEST_F(FTraceTest, testFunctionFilterAccessible) {
  testFunctionFilterAccessible();
}

TEST_F(FTraceTest, testTryStartTrace) {
  testTryStartTrace();
}

TEST_F(FTraceTest, testTryStopTrace) {
  testTryStopTrace();
}

TEST_F(FTraceTest, tryGetFunctionsFromFilter) {
  testTryGetFunctionsFromFilter();
}

TEST_F(FTraceTest, trySetBufferSize) {
  testTrySetBufferSize();
}

TEST_F(FTraceTest, hasTraceClockSetTo) {
  testHasTraceClockSetTo();
}