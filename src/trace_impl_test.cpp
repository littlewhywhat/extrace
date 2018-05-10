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

#include "trace_impl.h"
#include "mock_kernelsystem.h"
#include "mock_androidtracesystem.h"

#include <cstdio>

using ::testing::Return;
using ::testing::internal::CaptureStdout;
using ::testing::internal::GetCapturedStdout;

//! Tests TraceImplTest
class TraceImplTest : public ::testing::Test {
  public:
    //! tests setUp method of Trace
    void SetUp() {
      defaultBufferSize = 2048;
      kernelSystem = new MockKernelSystem();
      androidSystem = new MockAndroidTraceSystem();
      auto spKernelSystem = shared_ptr<KernelSystem>(kernelSystem);
      auto spAndroidTraceSystem = shared_ptr<AndroidTraceSystem>(androidSystem);
      trace.setErrorStream(stdout);
      trace.setKernelSystem(spKernelSystem);
      trace.setAndroidTraceSystem(spAndroidTraceSystem);
      CaptureStdout();
    }

    void testDefaultSetUp() {
      vector<string> emptyVector;
      EXPECT_CALL(*kernelSystem, setTraceOverwriteEnable(false))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setTraceBufferSizeKB(defaultBufferSize))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setGlobalClockEnable(true))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setPrintTgidEnableIfPresent(true))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setKernelTraceFuncs(emptyVector))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*androidSystem, tryEnableCategories(emptyVector))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*androidSystem, setAppCmdlineProperty(emptyVector))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*androidSystem, pokeBinderServices())
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, disableKernelTraceEvents())
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setKernelTraceCategories(emptyVector))
                                 .WillOnce(Return(true));
      ASSERT_TRUE(trace.setUp());
      EXPECT_EQ(GetCapturedStdout(), string());
    }
    //! tests setUp method of Trace
    void testNonEmptySetUp() {
      uint32_t bufferSize = 32;
      string function1 = "function1";
      string app1      = "app1";
      string android1  = "android1";
      string kernel1   = "kernel1";
      vector<string> funcs = { function1 };
      vector<string> apps  = { app1 };
      vector<string> androidCategories = { android1 };
      vector<string> kernelCategories  = { kernel1 };
      EXPECT_CALL(*kernelSystem, setTraceOverwriteEnable(true))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setTraceBufferSizeKB(bufferSize))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setGlobalClockEnable(true))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setPrintTgidEnableIfPresent(true))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setKernelTraceFuncs(funcs))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*androidSystem, tryEnableCategories(androidCategories))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*androidSystem, setAppCmdlineProperty(apps))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*androidSystem, pokeBinderServices())
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, disableKernelTraceEvents())
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setKernelTraceCategories(kernelCategories))
                                 .WillOnce(Return(true));
      trace.addFunction(function1.c_str());
      trace.addApp(app1.c_str());
      trace.addKernelCategory(kernel1.c_str());
      trace.addAndroidCategory(android1.c_str());
      trace.enableTraceOverwrite();
      trace.setTraceBufferSizeKB(bufferSize);
      ASSERT_TRUE(trace.setUp());
      EXPECT_EQ(GetCapturedStdout(), string());
    }
    // ! tests setUp method of Trace
    void testFailedSetUp() {
      vector<string> emptyVector;
      EXPECT_CALL(*kernelSystem, setTraceOverwriteEnable(false))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setTraceBufferSizeKB(defaultBufferSize))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setGlobalClockEnable(true))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setPrintTgidEnableIfPresent(true))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setKernelTraceFuncs(emptyVector))
                                 .WillOnce(Return(false));
      EXPECT_CALL(*androidSystem, tryEnableCategories(emptyVector))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*androidSystem, setAppCmdlineProperty(emptyVector))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*androidSystem, pokeBinderServices())
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, disableKernelTraceEvents())
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setKernelTraceCategories(emptyVector))
                                 .WillOnce(Return(true));
      ASSERT_FALSE(trace.setUp());
      EXPECT_EQ(GetCapturedStdout(), string("error TraceImpl::setUp\n"));
    }
    // ! tests cleanUp method of Trace
    void testCleanUp() {
      vector<string> emptyVector;
      EXPECT_CALL(*kernelSystem, disableKernelTraceEvents())
                                 .WillOnce(Return(true));
      EXPECT_CALL(*androidSystem, disableAllCategories())
                                 .Times(1);
      EXPECT_CALL(*androidSystem, clearAppProperties())
                                 .Times(1);
      EXPECT_CALL(*androidSystem, pokeBinderServices())
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setTraceOverwriteEnable(true))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setTraceBufferSizeKB(1))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setGlobalClockEnable(false))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setPrintTgidEnableIfPresent(false))
                                 .WillOnce(Return(true));
      EXPECT_CALL(*kernelSystem, setKernelTraceFuncs(emptyVector))
                                 .WillOnce(Return(true));
      trace.cleanUp();
      EXPECT_EQ(GetCapturedStdout(), string());
    }
    // ! tests start method of Trace
    void testStart() {
      EXPECT_CALL(*kernelSystem, setTracingEnabled(true))
                                 .WillOnce(Return(true));
      ASSERT_TRUE(trace.start());
      EXPECT_EQ(GetCapturedStdout(), string());
    }
    // ! tests start method of Trace
    void testFailStart() {
      EXPECT_CALL(*kernelSystem, setTracingEnabled(true))
                                 .WillOnce(Return(false));
      ASSERT_FALSE(trace.start());
      EXPECT_EQ(GetCapturedStdout(), string("error TraceImpl::start\n"));
    }
    // ! tests stop method of Trace
    void testStop() {
      EXPECT_CALL(*kernelSystem, setTracingEnabled(false))
                                 .WillOnce(Return(true));
      trace.stop();
      EXPECT_EQ(GetCapturedStdout(), string());
    }
  private:
    TraceImpl trace;
    MockAndroidTraceSystem * androidSystem;
    MockKernelSystem * kernelSystem;
    uint32_t defaultBufferSize;
};

TEST_F(TraceImplTest, defaultSetUp)
{
  testDefaultSetUp();
}

TEST_F(TraceImplTest, nonEmptySetUp)
{
  testNonEmptySetUp();
}

TEST_F(TraceImplTest, failedSetUp)
{
  testFailedSetUp();
}

TEST_F(TraceImplTest, cleanUp)
{
  testCleanUp();
}

TEST_F(TraceImplTest, start)
{
  testStart();
}

TEST_F(TraceImplTest, failStart)
{
  testFailStart();
}

TEST_F(TraceImplTest, stop)
{
  testStop();
}