/*
 * testLoop.cpp
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */
#include <stdio.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <tbsys/tblog.h>
#include "eventloop/ev_io.h"
#include "eventloop/ev_loop.h"
#include "eventloop/ev_event_mask.h"

class testClass: public testing::Test {
	virtual void SetUp()
	{
		TBSYS_LOG(INFO, "testClass setup function executed");
	}
};

static int func(ev_watcher *watcher, int revents)
{
	int ret = EV_SUCCESS;
	char *line = NULL;
	size_t linelen = 0;
	int read = getline(&line, &linelen, stdin);
	if (read < 0) {
		printf("could not getline\n");
		abort();
	} else {
		printf("Read: %.*s", read, line);
	}
	return ret;
}

static ev_io ev_io_obj(func, STDOUT_FILENO, EV_READ);
int main(int argc, char * argv[])
{
	ev_loop loop;
	EXPECT_EQ(EV_SUCCESS, loop.init_loop());
	TBSYS_LOG(INFO, "next ev_io_start");
	EXPECT_EQ(EV_SUCCESS, loop.ev_io_start(&ev_io_obj));
	TBSYS_LOG(INFO, "ev_io_start finished");

	return loop.run();
}
