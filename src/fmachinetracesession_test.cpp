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

#include "fmachinetracesession.h"
#include "mock_machinetrace.h"

using ::testing::Return;

//! Tests FMachineTraceSession
class FMachineTraceSessionTest : public ::testing::Test {
  public:
    //! tests start of FMachineTraceSession
    void test_start() {
      MockMachineTrace * machinetrace = new MockMachineTrace();
      EXPECT_CALL(*machinetrace, run())
                                 .Times(1);
      fmachinetracesession.set_machinetrace(machinetrace);
      fmachinetracesession.start();
    }
  private:
    //! Tested instance of FMachineTraceSession
    FMachineTraceSession fmachinetracesession;
};

TEST_F(FMachineTraceSessionTest, start)
{
  test_start();
}