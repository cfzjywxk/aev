/*
 * evl_oop.cpp
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#include "ev_loop.h"
#include <sys/epoll.h>
#include "lib/Common.h"
#include "lib/commonallocator.h"
#include "lib/tblog.h"
#include "ev_io.h"
#include "anfd.h"
#include "ev_event_mask.h"

ev_loop::ev_loop() :
		ev_backend_fd_(-1), ev_backended_(false), backend_mintime(0), epoll_events(
		NULL), epoll_eventmax(0), ev_timestamp_(0), loop_done_(false), fdchanges_(), fdmap_() {
	// TODO Auto-generated constructor stub

}

ev_loop::~ev_loop() {
	// TODO Auto-generated destructor stub
}

int ev_loop::init_loop()
{
	return epoll_backend(ev_backend_fd_, ev_backended_);
}

int ev_loop::epoll_backend(int &backended_fd, bool &res)
{
	int ret = EV_SUCCESS;
#ifdef EPOLL_CLOEXEC
  backended_fd = epoll_create1 (EPOLL_CLOEXEC);

  if (backended_fd < 0 && (errno == EINVAL || errno == ENOSYS))
#endif
    backended_fd = epoll_create (256); //max 256 fd

  if (backended_fd < 0) {
  	ret = EV_INVALID_VALUE;
  }

  if (EV_SUCCESS == ret) {
	  fcntl (backended_fd, F_SETFD, FD_CLOEXEC);

	  backend_mintime = 1e-3; /* epoll does sometimes return early, this is just to avoid the worst */
	  epoll_eventmax = 64; /* initial number of events receivable per poll */
	  if ((EV_SUCCESS != (ret = tc_malloc(sizeof(struct epoll_event), epoll_events)))) {
	  	TBSYS_LOG(ERROR, "alloc %ld mem for epoll_event failed ret=%d\n", sizeof(struct epoll_event), ret);
	  }
  }

	if (EV_SUCCESS == ret) {
		res = true;
	} else {
		res = false;
	}
	return ret;
}

int ev_loop::run()
{
	int ret = EV_SUCCESS;
	while (!loop_done_ && ret == EV_SUCCESS) {
		//1. fd_reify
		//2. epoll_wait
		//3. execute callback function
		if (EV_SUCCESS != (ret = fd_reify())) {
			TBSYS_LOG(WARN, "fd_reify failed ret=%d\n", ret);
		} else if (EV_SUCCESS != (ret = polling())) {
			TBSYS_LOG(WARN, "polling failed ret=%d\n", ret);
		} else if (EV_SUCCESS != (ret = invoke_pending())) {
			TBSYS_LOG(WARN, "invoke pending failed ret=%d\n", ret);
		}
	}
	return ret;
}


int ev_loop::ev_io_start(ev_io *event_io)
{
	int ret = EV_SUCCESS;
	if (NULL == event_io) {
		ret = EV_INVALID_ARGS;
	} else {
		return fd_change(event_io->get_fd(), event_io->get_events());
	}
	return ret;
}


int ev_loop::fd_change(int fd, int revents)
{
	//TODO
	int ret = EV_SUCCESS;
	fdchanges_.push_back(fd);
	return ret;
}

int ev_loop::fd_reify()
{
	int ret = EV_SUCCESS;
	int fdchanges_cnt = fdchanges_.size();
	for (int i = 0; i < fdchanges_cnt; ++i) {
		int fd = fdchanges_[i];
		anfd *an_fd = NULL;
		if (fdmap_.count(fd) == 0) {
			NEW(anfd, an_fd, sizeof(anfd));
			if (NULL != an_fd) {
				fdmap_.insert( { fd, an_fd });
			} else {
				ret = EV_ALLOC_FAILED;
			}
		} else {
			anfd *an_fd = fdmap_.at(fd);
		}
		ev_io *w = NULL;
		if (EV_SUCCESS == ret) {
			unsigned char o_events = an_fd->events_;
			unsigned char o_reify = an_fd->reify_;
			an_fd->reify_ = 0;
			an_fd->events_ = 0;

			//walk through the watcher list and collect all the events flags
			for (std::list<ev_io*>::iterator it = an_fd->watcher_list_.begin();
					it != an_fd->watcher_list_.end(); ++it) {
				an_fd->events_ |= (unsigned char) (*it)->get_events();
			}
			if (o_events != an_fd->events_) {
				o_reify = EV__IOFDSET; /* actually |= */
			}
			if (o_reify & EV__IOFDSET) {
				if (EV_SUCCESS != (ret = backend_modify(fd, o_events, an_fd->events_))) {
					TBSYS_LOG(INFO, "backend modify failed ret=%d\n", ret);
				}
			}
		}
	}
	return ret;
}

int ev_loop::backend_modify(int fd, unsigned char o_events, unsigned char cur_events)
{
	int ret = EV_SUCCESS;
	return ret;
}

int ev_loop::polling()
{
	int ret = EV_SUCCESS;
	return ret;
}

int ev_loop::invoke_pending()
{
	int ret = EV_SUCCESS;
	return ret;
}
