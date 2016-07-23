/*
 * test_ev_io.cpp
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */


#include <iostream>
#include <gtest/gtest.h>
#include <unistd.h>
#include "ev_io.h"
#include "lib/tblog.h"

class testClass : public testing::Test {
	virtual void SetUp() {
		TBSYS_LOG(INFO, "testClass setup function executed");
	}
};


int func(ev_loop *loop, ev_watcher *watcher, int revents)
{
	int ret = EV_SUCCESS;
	std::cout << "func called" << std::endl << std::cout;
	return ret;
}

TEST_F(testClass, test_ev_io_basic)
{
	ev_io ev_io_obj(func, STDOUT_FILENO, 1);
  EXPECT_EQ(EV_SUCCESS, ev_io_obj.callback(NULL, 1));
}

int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

