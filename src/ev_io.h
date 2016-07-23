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

class ev_io : public ev_watcher {
public:
	ev_io(CallbackPtr ptr) : ev_watcher(ptr), fd_(EV_INVALID_VALUE), events_(EV_INVALID_VALUE){};
	ev_io(CallbackPtr ptr, int fd, int events) : ev_watcher(ptr), fd_(fd), events_(events){};
	virtual ~ev_io();

	inline int get_fd() {return fd_;}
	inline void set_fd(int fd) {fd_ = fd;}

	inline int get_events() {return events_;}
	inline void set_events(int events) {events_ = events;}

protected:
	int fd_;
	int events_;
};

#endif /* EVIO_H_ */
