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

#include "extraceapp.h"

#include "systemtime_impl.h"
#include "kernelsystem_impl.h"
#include "androidtoolbox.h"
#include "trace_impl.h"
#include "signal.h"
#include "startaction.h"
#include "stopaction.h"
#include "sleepaction.h"
#include "dumpaction.h"
#include "streamaction.h"
#include "cleanupaction.h"
#include "actionrunner_impl.h"

#include "mock_filesystem_impl.h"
#include "mock_androidsystem_impl.h"

#include <gtest/gtest.h>

using namespace std;
using ::testing::Return;
using ::testing::_;
using ::testing::StrEq;

//! Tests ArgsParser
class ExtraceAppTest : public ::testing::Test {
  public:
    void SetUp() {
      m_ExtraceApp.setErrorStream(stderr);
      m_ExtraceApp.setOutputStream(stdout);
      m_ExtraceApp.setSystemTimeImpl(new SystemTimeImpl());
      m_ExtraceApp.setKernelSystemImpl(new KernelSystemImpl());
      m_ExtraceApp.setTraceImpl(new TraceImpl());
      m_ExtraceApp.setInitSleepAction(new SleepAction());
      m_ExtraceApp.setStartAction(new StartAction());
      m_ExtraceApp.setMidSleepAction(new SleepAction());
      m_ExtraceApp.setStreamAction(new StreamAction());
      m_ExtraceApp.setStopAction(new StopAction());
      m_ExtraceApp.setDumpAction(new DumpAction());
      m_ExtraceApp.setCleanUpAction(new CleanUpAction());
      m_ExtraceApp.setSignal(new Signal());
    }

    void testRun() {
      MockFileSystemImpl * fileSystemImpl = new MockFileSystemImpl();
      EXPECT_CALL(*fileSystemImpl, 
                  writeStr(StrEq("/sys/kernel/debug/tracing/options/overwrite"),
                           StrEq("0")))
                  .WillOnce(Return(true));
      EXPECT_CALL(*fileSystemImpl,
                  writeStr(StrEq("/sys/kernel/debug/tracing/buffer_size_kb"),
                           StrEq("2048")))
                  .WillOnce(Return(true));
      EXPECT_CALL(*fileSystemImpl,
                  readStr(StrEq("/sys/kernel/debug/tracing/trace_clock"),
                          _, 4097))
                  .WillOnce(Return(true));
      EXPECT_CALL(*fileSystemImpl,
                  writeStr(StrEq("/sys/kernel/debug/tracing/trace_clock"),
                           StrEq("global")))
                  .WillOnce(Return(true));
      EXPECT_CALL(*fileSystemImpl,
                  fileExists(StrEq("/sys/kernel/debug/tracing/options/print-tgid")))
                  .WillOnce(Return(true));
      EXPECT_CALL(*fileSystemImpl,
                  fileIsWritable(StrEq("/sys/kernel/debug/tracing/current_tracer")))
                  .WillOnce(Return(true));
      m_ExtraceApp.setFileSystemImpl(fileSystemImpl);
      m_ExtraceApp.setAndroidSystemImpl(new MockAndroidSystemImpl());

      int argc = 1;
      const char * argv[] = { "extrace" };
      ASSERT_TRUE(m_ExtraceApp.run(argc, argv));
    }
  private:
    ExtraceApp m_ExtraceApp;
};

TEST_F(ExtraceAppTest, run) {
  testRun();
}