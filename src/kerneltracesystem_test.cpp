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
   This is a template for test of KernelTraceSystem.
   Things to substitute:
    - KernelTraceSystem, kerneltracesystem
    - FTrace, ftrace, ftraceTryEnableCategory
    - TryEnableCategory, tryEnableCategory, 
   After test is run and fails with testThatIrun,
   remove it and uncomment other methods
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "kerneltracesystem.h"
#include "mock_ftrace.h"

#include <memory>

using namespace std;
using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::SetArrayArgument;

//! I am a test of KernelTraceSystem. I can test its methods.
class KernelTraceSystemTest : public ::testing::Test {
  public:
    //! Setups my resources
    void SetUp() {
      myWire = new Wire(stderr, stderr);
      myMockFTrace = new MockFTrace();
      auto sp_MockFTrace = shared_ptr<MockFTrace>(myMockFTrace);
      myKernelTraceSystem = new KernelTraceSystem(*myWire, sp_MockFTrace);
    }

    //! Tears down my resources
    void TearDown() {
      // assumed here that MockFTrace is managed by KernelTraceSystem
      delete myKernelTraceSystem;
      delete myWire;
      myKernelTraceSystem = NULL;
    }

    //! Tests KernelTraceSystem's supportsCategory method to return true
    void testSupportsCategorySCHEDTrue() {
      EXPECT_CALL(*myMockFTrace,
                  tracePointAccessable(FTrace::TracePoint::SCHED_SWITCH))
                                       .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tracePointAccessable(FTrace::TracePoint::SCHED_WAKEUP))
                                       .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tracePointAccessable(FTrace::TracePoint::SCHED_CPU_HOTPLUG))
                                      .WillOnce(Return(false));
      EXPECT_CALL(*myMockFTrace,
                  tracePointAccessable(FTrace::TracePoint::SCHED_BLOCKED_REASON))
                                      .WillOnce(Return(false));
      EXPECT_TRUE(myKernelTraceSystem
        ->supportsCategory(KernelTraceSystem::TraceCategory::SCHED));
    }

    //! Tests KernelTraceSystem's supportsCategory method to return false
    void testSupportsCategorySCHEDFalse() {
      EXPECT_CALL(*myMockFTrace,
                  tracePointAccessable(FTrace::TracePoint::SCHED_SWITCH))
                                       .WillOnce(Return(false));
      EXPECT_FALSE(myKernelTraceSystem
        ->supportsCategory(KernelTraceSystem::TraceCategory::SCHED));
    }

    //! Tests KernelTraceSystem's trySetFunctions method to return true
    void testTrySetFunctionsValid() {
      set<string> functions = { "function1", "function2" };
      EXPECT_CALL(*myMockFTrace,
                  trySetTracer(FTrace::Tracer::FUNCTION_GRAPH))
                                       .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryEnableOption(FTrace::Option::FUNCGRAPH_ABSTIME))
                                       .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryEnableOption(FTrace::Option::FUNCGRAPH_CPU))
                                       .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryEnableOption(FTrace::Option::FUNCGRAPH_PROC))
                                       .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryClearFunctionFilter()).WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryAddFunctionToFilter(StrEq("function1")))
                  .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryAddFunctionToFilter(StrEq("function2")))
                  .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryGetFunctionsFromFilter())
                  .WillOnce(Return(functions));
      EXPECT_TRUE(myKernelTraceSystem->trySetFunctions(functions));
    }

    //! Tests KernelTraceSystem's trySetFunctions method to return true
    void testTrySetFunctionsInvalid() {
      set<string> functions = { "function1", "function2" };
      EXPECT_CALL(*myMockFTrace,
                  trySetTracer(FTrace::Tracer::FUNCTION_GRAPH))
                                       .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryEnableOption(FTrace::Option::FUNCGRAPH_ABSTIME))
                                       .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryEnableOption(FTrace::Option::FUNCGRAPH_CPU))
                                       .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryEnableOption(FTrace::Option::FUNCGRAPH_PROC))
                                       .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryClearFunctionFilter()).WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryAddFunctionToFilter(StrEq("function1")))
                  .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryAddFunctionToFilter(StrEq("function2")))
                  .WillOnce(Return(true));
      EXPECT_CALL(*myMockFTrace,
                  tryGetFunctionsFromFilter())
                  .WillOnce(Return(set<string>()));
      EXPECT_FALSE(myKernelTraceSystem->trySetFunctions(functions));
    }
  private:
    //! Tested instance of KernelTraceSystem
    KernelTraceSystem * myKernelTraceSystem = NULL;
    MockFTrace * myMockFTrace = NULL;
    Wire * myWire = NULL;

};

TEST_F(KernelTraceSystemTest, testSupportsCategorySCHEDTrue)
{
  testSupportsCategorySCHEDTrue();
}

TEST_F(KernelTraceSystemTest, testSupportsCategorySCHEDFalse)
{
  testSupportsCategorySCHEDFalse();
}

TEST_F(KernelTraceSystemTest, testTrySetFunctionsValid)
{
  testTrySetFunctionsValid();
}

TEST_F(KernelTraceSystemTest, testTrySetFunctionsInvalid)
{
  testTrySetFunctionsInvalid();
}