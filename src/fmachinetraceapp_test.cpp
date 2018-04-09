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

#include "fmachinetraceapp.h"
#include "mock_machinetracesession.h"

using ::testing::Return;

//! Tests FMachineTraceApp
class FMachineTraceAppTest : public ::testing::Test {
  public:
    //! tests run of FMachineTraceApp
    void test_run() {
      MockMachineTraceSession * machinetracesession = new MockMachineTraceSession();
      EXPECT_CALL(*machinetracesession, start())
                                        .Times(1);
      fmachinetraceapp.set_machinetracesession(machinetracesession);
      fmachinetraceapp.run();
      ASSERT_TRUE(fmachinetraceapp.succeeded());
    }
  private:
    //! Tested instance of FMachineTraceApp
    FMachineTraceApp fmachinetraceapp;
};

TEST_F(FMachineTraceAppTest, run)
{
  test_run();
}