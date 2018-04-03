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
#include <cstdlib>

#include "consoleappmain.h"
#include "mock_consoleapp.h"

using ::testing::Return;
using ::testing::_;

//! Tests ConsoleAppMain
class ConsoleAppMainTest : public ::testing::Test {
  public:
    //! tests successful run of app
    void test_run() {
      MockConsoleApp * consoleapp = new MockConsoleApp();
      EXPECT_CALL(*consoleapp, setup(_,_))
                              .Times(1);
      EXPECT_CALL(*consoleapp, is_ready())
                              .Times(1)
                              .WillOnce(Return(true));
      EXPECT_CALL(*consoleapp, run())
                              .Times(1);
      EXPECT_CALL(*consoleapp, succeeded())
                              .Times(1)
                              .WillOnce(Return(true));
      consoleAppMain.set_consoleapp(consoleapp);

      int argc = 1;
      const char * argv[] = { "ConsoleAppMainTest" };
      ASSERT_EQ(consoleAppMain.run(argc, argv), EXIT_SUCCESS);
    }
    //! tests if app is not ready after setup
    void test_failed_ready_run() {
      MockConsoleApp * consoleapp = new MockConsoleApp();
      EXPECT_CALL(*consoleapp, setup(_,_))
                              .Times(1);
      EXPECT_CALL(*consoleapp, is_ready())
                              .Times(1)
                              .WillOnce(Return(false));
      EXPECT_CALL(*consoleapp, run())
                              .Times(0);
      EXPECT_CALL(*consoleapp, succeeded())
                              .Times(0);
      consoleAppMain.set_consoleapp(consoleapp);

      int argc = 1;
      const char * argv[] = { "ConsoleAppMainTest" };
      ASSERT_EQ(consoleAppMain.run(argc, argv), EXIT_FAILURE);
    }
    //! tests unsuccessful run of app
    void test_failed_run() {
      MockConsoleApp * consoleapp = new MockConsoleApp();
      EXPECT_CALL(*consoleapp, setup(_,_))
                              .Times(1);
      EXPECT_CALL(*consoleapp, is_ready())
                              .Times(1)
                              .WillOnce(Return(true));
      EXPECT_CALL(*consoleapp, run())
                              .Times(1);
      EXPECT_CALL(*consoleapp, succeeded())
                              .Times(1)
                              .WillOnce(Return(false));
      consoleAppMain.set_consoleapp(consoleapp);

      int argc = 1;
      const char * argv[] = { "ConsoleAppMainTest" };
      ASSERT_EQ(consoleAppMain.run(argc, argv), EXIT_FAILURE);
    }
  private:
    //! Tested instance
    ConsoleAppMain consoleAppMain;
};

TEST_F(ConsoleAppMainTest, run)
{
  test_run();
}

TEST_F(ConsoleAppMainTest, failed_ready_run)
{
  test_failed_ready_run();
}

TEST_F(ConsoleAppMainTest, failed_run)
{
  test_failed_run();
}