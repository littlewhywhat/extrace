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

#include "extraceconsoleapp.h"
#include "mock_machinetraceapp.h"

using ::testing::Return;

//! Tests ExtraceConsoleApp
class ExtraceConsoleAppTest : public ::testing::Test {
  public:
    //! tests method of ExtraceConsoleApp
    void test_run() {
      MockMachineTraceApp * machinetraceapp = new MockMachineTraceApp();
      EXPECT_CALL(*machinetraceapp, run())
                                    .Times(1);
      EXPECT_CALL(*machinetraceapp, succeeded())
                                    .Times(1)
                                    .WillOnce(Return(true));
      extraceconsoleapp.set_machinetraceapp(machinetraceapp);
      extraceconsoleapp.run();
      ASSERT_TRUE(extraceconsoleapp.succeeded());
    }
  private:
    //! Tested instance of ExtraceConsoleApp
    ExtraceConsoleApp extraceconsoleapp;
};

TEST_F(ExtraceConsoleAppTest, run)
{
  test_run();
}