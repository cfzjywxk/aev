/*
 * EVIO.h
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#ifndef EVIO_H_
#define EVIO_H_

#include "ev_watcher.h"
#include "lib/Common.h"

class ev_io : public ev_watcher{
public:
	ev_io() : fd_(EV_INVALID_VALUE), events_(EV_INVALID_VALUE){};
	ev_io(int fd, int events) : fd_(fd), events_(events){};
	virtual ~ev_io();

protected:
	int fd_;
	int events_;
};

#endif /* EVIO_H_ */
