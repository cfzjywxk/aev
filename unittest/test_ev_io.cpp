/*
 * test_ev_io.cpp
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#include <iostream>
#include <unistd.h>
#include <gtest/gtest.h>
#include "deps/tbsys/src/tblog.h"
#include "eventloop/ev_io.h"

class testClass : public testing::Test {
  virtual void SetUp() {
    TBSYS_LOG(INFO, "testClass setup function executed");
  }
};

static int func(ev_watcher *watcher, int revents) {
  int ret = EV_SUCCESS;
  std::cout << "func called" << std::endl;
  return ret;
}

TEST_F(testClass, test_ev_io_basic) {
  ev_io ev_io_obj(func, STDOUT_FILENO, 1);
  EXPECT_EQ(EV_SUCCESS, ev_io_obj.callback(1));
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

