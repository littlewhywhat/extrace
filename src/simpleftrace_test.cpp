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

//! I am a test of SimpleFTrace. I can test its methods.
class SimpleFTraceTest : public ::testing::Test {
  public:
    //! Setups my resources
    void SetUp() {
      myMockFileSystem = new MockFileSystem();
      myMockToolBox = new MockToolBox();
      auto sp_myMockFileSystem = shared_ptr<FileSystem>(myMockFileSystem);
      auto sp_myMockToolBox = shared_ptr<ToolBox>(myMockToolBox);
      myWire = new Wire(stdout,stdout);
      mySimpleFTrace = new SimpleFTrace(*myWire,
                            sp_myMockFileSystem,
                            sp_myMockToolBox,
                            mySimpleFTraceMountPoint);
    }

    //! Tears down my resources
    void TearDown() {
      // assumed here that MockFileSystem is managed by SimpleFTrace
      delete mySimpleFTrace;
      delete myWire;
      myMockFileSystem = NULL;
      mySimpleFTrace = NULL;
    }

    //! Tests SimpleFTrace's tryEnableTracePoint method with accessable tracepoint
    void testTryEnableAccessableTracePoint(const SimpleFTrace::TracePoint & tracePoint,
                                           const char * tracePointPath) {
      EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(mySimpleFTraceMountPoint
                                                    + tracePointPath), StrEq("1")))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(_))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(mySimpleFTrace->tryEnableTracePoint(tracePoint));
    }

    //! Tests SimpleFTrace's tryEnableTracePoint method with not accessable tracepoint
    void testTryEnableNotAccessableTracePoint(const SimpleFTrace::TracePoint & tracePoint) {
      EXPECT_CALL(*myMockFileSystem, writeStr(_,_))
                                      .Times(0);
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(_))
                                      .WillOnce(Return(false));
      EXPECT_FALSE(mySimpleFTrace->tryEnableTracePoint(tracePoint));
    }

    //! Tests SimpleFTrace's tryCleanTrace method
    void testTryCleanTrace() {
      EXPECT_CALL(*myMockFileSystem, truncateFile(StrEq(mySimpleFTraceMountPoint
                                                        + "/tracing/trace")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(mySimpleFTrace->tryCleanTrace());
    }
    //! Tests SimpleFTrace's tracePointAccessable method
    void testTracePointAccessable(const SimpleFTrace::TracePoint & tracePoint,
                                  const char * tracePointPath) {
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(StrEq(mySimpleFTraceMountPoint
                                                          + tracePointPath)))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(mySimpleFTrace->tracePointAccessable(tracePoint));
    }
    //! Tests SimpleFTrace's tracePointExists method
    void testTracePointExists(const SimpleFTrace::TracePoint & tracePoint,
                              const char * tracePointPath) {
      EXPECT_CALL(*myMockFileSystem, fileExists(StrEq(mySimpleFTraceMountPoint
                                                      + tracePointPath)))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(mySimpleFTrace->tracePointExists(tracePoint));
    }
    //! Tests SimpleFTrace's tryWriteMarker method
    void testTryWriteMarker(const char * marker) {
       EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(mySimpleFTraceMountPoint
                                                     + "/tracing/trace_marker"),
                                               StrEq(marker)))
                                      .WillOnce(Return(true));
       EXPECT_TRUE(mySimpleFTrace->tryWriteMarker(marker));
    }
    //! Tests SimpleFTrace's trySetTracer method
    void testTrySetTracer(const SimpleFTrace::Tracer & tracer, const char * tracerName) {
       EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(mySimpleFTraceMountPoint
                                                     + "/tracing/current_tracer"),
                                               StrEq(tracerName)))
                                      .WillOnce(Return(true));
       EXPECT_TRUE(mySimpleFTrace->trySetTracer(tracer));
    }

    //! Tests SimpleFTrace's tryClearFunctionFilter method
    void testTryClearFunctionFilter() {
      EXPECT_CALL(*myMockFileSystem, truncateFile(StrEq(mySimpleFTraceMountPoint
                                                     + "/tracing/set_ftrace_filter")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(mySimpleFTrace->tryClearFunctionFilter());
    }
    //! Tests SimpleFTrace's tryAddFunctionToFilter method
    void testTryAddFunctionToFilter(const char * function) {
       EXPECT_CALL(*myMockFileSystem, appendStr(StrEq(mySimpleFTraceMountPoint
                                                     + "/tracing/set_ftrace_filter"),
                                                StrEq(function)))
                                      .WillOnce(Return(true));
       EXPECT_TRUE(mySimpleFTrace->tryAddFunctionToFilter(function));
    }
    //! Tests SimpleFTrace's tracerChoiceAccessable method
    void testTracerChoiceAccessable() {
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(StrEq(mySimpleFTraceMountPoint
                                                     + "/tracing/current_tracer")))
                                     .WillOnce(Return(true));
      EXPECT_TRUE(mySimpleFTrace->tracerChoiceAccessable());
    }
    //! Tests SimpleFTrace's tunctionFilterAccessible method
    void testFunctionFilterAccessible() {
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(StrEq(mySimpleFTraceMountPoint
                                                     + "/tracing/set_ftrace_filter")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(mySimpleFTrace->functionFilterAccessible());
    }

    //! Tests SimpleFTrace's tryStart
    void testTryStartTrace() {
      EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(mySimpleFTraceMountPoint
                                                    + "/tracing/tracing_on"),
                                              StrEq("1")))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(StrEq(mySimpleFTraceMountPoint
                                                    + "/tracing/tracing_on")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(mySimpleFTrace->tryStartTrace());
    }

    //! Tests SimpleFTrace's tryStop
    void testTryStopTrace() {
      EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(mySimpleFTraceMountPoint
                                                    + "/tracing/tracing_on"),
                                              StrEq("0")))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(StrEq(mySimpleFTraceMountPoint
                                                    + "/tracing/tracing_on")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(mySimpleFTrace->tryStopTrace());
    }

    //! Tests SimpleFTrace's tryGetFunctionsFromFilter
    void testTryGetFunctionsFromFilter() {
      const char * functionFilterContent = "function1\nwild*card\nfunction2\n";
      EXPECT_CALL(*myMockFileSystem, readStr(StrEq(mySimpleFTraceMountPoint
                                                     + "/tracing/set_ftrace_filter"),
                                             _, 4097))
                                      .WillOnce(
                                        DoAll(
                                          SetArrayArgument<1>(functionFilterContent,
                                                              functionFilterContent
                                                              + strlen(functionFilterContent) + 1), 
                                                      Return(true)));
      set<string> tokens = { "function1", "wild*card", "function2" };
      EXPECT_CALL(*myMockToolBox, parseToTokens(StrEq(functionFilterContent),
                                                StrEq("\n"), _))
                  .WillOnce(SetArgReferee<2>(tokens));
      const set<string> functions = mySimpleFTrace->tryGetFunctionsFromFilter();
      tokens.erase("wild*card");
      EXPECT_EQ(functions, tokens);
    }

    //! Tests SimpleFTrace's trySetBufferSize
    void testTrySetBufferSize() {
      EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(mySimpleFTraceMountPoint
                                                    + "/tracing/buffer_size_kb"),
                                              StrEq("4294967295")))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(mySimpleFTrace->trySetBufferSize(UINT32_MAX));
      EXPECT_FALSE(mySimpleFTrace->trySetBufferSize(0));
    }

    //! Tests SimpleFTrace's tryEnableOption nethod with accessible option
    void testTryEnableAccessableOption(const SimpleFTrace::Option & option,
                                       const char * optionPath) {
      EXPECT_CALL(*myMockFileSystem, writeStr(StrEq(mySimpleFTraceMountPoint
                                                    + optionPath), StrEq("1")))
                                      .WillOnce(Return(true));
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(_))
                                      .WillOnce(Return(true));
      EXPECT_TRUE(mySimpleFTrace->tryEnableOption(option));
    }

    //! Tests SimpleFTrace's tryEnableOption method with not accessable tracepoint
    void testTryEnableNotAccessableOption(const SimpleFTrace::Option & option) {
      EXPECT_CALL(*myMockFileSystem, writeStr(_,_))
                                      .Times(0);
      EXPECT_CALL(*myMockFileSystem, fileIsWritable(_))
                                      .WillOnce(Return(false));
      EXPECT_FALSE(mySimpleFTrace->tryEnableOption(option));
    }

    //! Tests SimpleFTrace's hasTraceClockSetTo
    void testHasTraceClockSetTo() {
      const char * traceClockContent = "global [local]";
      EXPECT_CALL(*myMockFileSystem, readStr(StrEq(mySimpleFTraceMountPoint
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
      EXPECT_FALSE(mySimpleFTrace->hasTraceClockSetTo(SimpleFTrace::ClockType::GLOBAL));
      EXPECT_TRUE(mySimpleFTrace->hasTraceClockSetTo(SimpleFTrace::ClockType::LOCAL));
    }
  private:
    //! Tested instance of SimpleFTrace
    SimpleFTrace * mySimpleFTrace = NULL;
    MockFileSystem * myMockFileSystem = NULL;
    MockToolBox * myMockToolBox = NULL;
    Wire * myWire = NULL;
    const string mySimpleFTraceMountPoint = "/sys/debug";
};

TEST_F(SimpleFTraceTest, testTryEnableAccessableTracePoint)
{
  testTryEnableAccessableTracePoint(SimpleFTrace::TracePoint::SCHED_SWITCH,
                                    "/tracing/events/sched/sched_switch/enable");
}

TEST_F(SimpleFTraceTest, testTryEnableNotAccessableTracePoint)
{
  testTryEnableNotAccessableTracePoint(SimpleFTrace::TracePoint::SCHED_SWITCH);
}

TEST_F(SimpleFTraceTest, testTryEnableAccessableOption)
{
  testTryEnableAccessableOption(SimpleFTrace::Option::OVERWRITE,
                                "/tracing/options/overwrite");
}

TEST_F(SimpleFTraceTest, testTryEnableNotAccessableOption)
{
  testTryEnableNotAccessableOption(SimpleFTrace::Option::OVERWRITE);
}

TEST_F(SimpleFTraceTest, testTryCleanTrace) {
  testTryCleanTrace();
}

TEST_F(SimpleFTraceTest, testTracePointAccessable) {
  testTracePointAccessable(SimpleFTrace::TracePoint::SCHED_SWITCH,
                           "/tracing/events/sched/sched_switch/enable");
}

TEST_F(SimpleFTraceTest, testTracePointExists) {
  testTracePointExists(SimpleFTrace::TracePoint::SCHED_SWITCH,
                       "/tracing/events/sched/sched_switch/enable");
}

TEST_F(SimpleFTraceTest, testTryWriteMarker) {
  testTryWriteMarker("marker");
}

TEST_F(SimpleFTraceTest, testTrySetTracer) {
  testTrySetTracer(SimpleFTrace::Tracer::NOP, "nop");
}

TEST_F(SimpleFTraceTest, testTryClearFunctionFilter) {
  testTryClearFunctionFilter();
}

TEST_F(SimpleFTraceTest, testTryAddFunctionToFilter) {
  testTryAddFunctionToFilter("function_one");
}

TEST_F(SimpleFTraceTest, testTracerChoiceAccessable) {
  testTracerChoiceAccessable();
}

TEST_F(SimpleFTraceTest, testFunctionFilterAccessible) {
  testFunctionFilterAccessible();
}

TEST_F(SimpleFTraceTest, testTryStartTrace) {
  testTryStartTrace();
}

TEST_F(SimpleFTraceTest, testTryStopTrace) {
  testTryStopTrace();
}

TEST_F(SimpleFTraceTest, tryGetFunctionsFromFilter) {
  testTryGetFunctionsFromFilter();
}

TEST_F(SimpleFTraceTest, trySetBufferSize) {
  testTrySetBufferSize();
}

TEST_F(SimpleFTraceTest, hasTraceClockSetTo) {
  testHasTraceClockSetTo();
}