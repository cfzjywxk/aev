/*
 * evl_oop.cpp
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#include "ev_loop.h"
#include <sys/epoll.h>
#include <assert.h>
#include "lib/Common.h"
#include "lib/commonallocator.h"
#include "lib/tblog.h"
#include "ev_io.h"
#include "anfd.h"
#include "ev_event_mask.h"

ev_loop::ev_loop() :
		ev_backend_fd_(-1), ev_backended_(false), backend_mintime(0), epoll_events(
		NULL), epoll_eventmax(0), epoll_epermcnt(false), ev_timestamp_(0), loop_done_(false), fdchanges_(), fdmap_(), pendings_()
{
	// TODO Auto-generated constructor stub

}

ev_loop::~ev_loop()
{
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
	backended_fd = epoll_create1(EPOLL_CLOEXEC);

	if (backended_fd < 0 && (errno == EINVAL || errno == ENOSYS))
#endif
		backended_fd = epoll_create(256); //max 256 fd

	if (backended_fd < 0) {
		ret = EV_INVALID_VALUE;
	}

	if (EV_SUCCESS == ret) {
		fcntl(backended_fd, F_SETFD, FD_CLOEXEC);

		backend_mintime = 1e-3; /* epoll does sometimes return early, this is just to avoid the worst */
		epoll_eventmax = 64; /* initial number of events receivable per poll */
		if ((NULL == (epoll_events = (struct epoll_event *) (tc_malloc(sizeof(struct epoll_event)))))) {
			TBSYS_LOG(ERROR, "alloc %ld mem for epoll_event failed ret=%d\n", sizeof(struct epoll_event),
					ret);
		}
	}

	if (EV_SUCCESS == ret) {
		res = true;
		TBSYS_LOG(INFO, "epoll create success epoll_fd=%d ", backended_fd);
	} else {
		res = false;
	}
	return ret;
}

int ev_loop::epoll_modify(int fd, int old_ev, int new_ev)
{
	int ret = EV_SUCCESS;
	struct epoll_event ev;
	unsigned char oldmask;

	/*
	 * we handle EPOLL_CTL_DEL by ignoring it here
	 * on the assumption that the fd is gone anyways
	 * if that is wrong, we have to handle the spurious
	 * event in epoll_poll.
	 * if the fd is added again, we try to ADD it, and, if that
	 * fails, we assume it still has the same eventmask.
	 */
	if (new_ev) {
		anfd *an_fd = NULL;
		if (fdmap_.count(fd) == 0) {
			ret = EV_INVALID_ARGS;
			TBSYS_LOG(ERROR, "invalid fd which cannot be found in map fd=%d ret=%d", fd, ret);
		} else {
			an_fd = fdmap_.at(fd);
		}
		oldmask = an_fd->emask_;
		an_fd->emask_ = new_ev;

		/* store the generation counter in the upper 32 bits, the fd in the lower 32 bits */
		ev.data.u64 = (uint64_t) (uint32_t) fd | ((uint64_t) (uint32_t) ++an_fd->egen_ << 32);
		ev.events = (new_ev & EV_READ ? EPOLLIN : 0) | (new_ev & EV_WRITE ? EPOLLOUT : 0);

		if (epoll_ctl(ev_backend_fd_, old_ev && oldmask != new_ev ? EPOLL_CTL_MOD : EPOLL_CTL_ADD, fd,
				&ev)) {
			ret = EV_EPOLL_FAIL;
			TBSYS_LOG(ERROR, "epoll_ctrl call failed on ev_backend_fd_=%d\n fd=%d, ret=%d",
					ev_backend_fd_, fd, ret);

			//error process
			if (errno == ENOENT) {
				/* if ENOENT then the fd went away, so try to do the right thing */
				if (!new_ev)
					goto dec_egen;

				if (!epoll_ctl(ev_backend_fd_, EPOLL_CTL_ADD, fd, &ev)) {
					ret = EV_SUCCESS;
				} else {
					ret = EV_EPOLL_FAIL;
					TBSYS_LOG(ERROR, "epoll_ctrl call failed AGAIN on ev_backend_fd_=%d\n fd=%d, ret=%d",
							ev_backend_fd_, fd, ret);
				}
			} else if ((errno == EEXIST)) {
				/* EEXIST means we ignored a previous DEL, but the fd is still active */
				/* if the kernel mask is the same as the new mask, we assume it hasn't changed */
				if (oldmask == new_ev)
					goto dec_egen;

				if (!epoll_ctl(ev_backend_fd_, EPOLL_CTL_MOD, fd, &ev)) {
					ret = EV_SUCCESS;
				} else {
					ret = EV_EPOLL_FAIL;
					TBSYS_LOG(ERROR, "epoll_ctrl call failed AGAIN on ev_backend_fd_=%d\n fd=%d, ret=%d",
							ev_backend_fd_, fd, ret);
				}
			} else if ((errno == EPERM)) {
				/* EPERM means the fd is always ready, but epoll is too snobbish */
				/* to handle it, unlike select or poll. */
#define EV_EMASK_EPERM 0x80
				an_fd->emask_ = EV_EMASK_EPERM;

				/* add fd to epoll_eperms, if not already inside */
				if (!(oldmask & EV_EMASK_EPERM)) {
					/*
					 array_needsize (int, epoll_eperms, epoll_epermmax, epoll_epermcnt + 1, EMPTY2);
					 epoll_eperms[epoll_epermcnt++] = fd;
					 */
				}
				TBSYS_LOG(ERROR, "seems fd not suported by epoll fd=%d, ret=%d", fd, ret);
			}
			fd_kill(an_fd);

			dec_egen:
			/* we didn't successfully call epoll_ctl, so decrement the generation counter again */
			--an_fd->egen_;
		}
	}

	return ret;
}

int ev_loop::fd_kill(anfd *afd)
{
	int ret = EV_SUCCESS;
	//walk through the watcher list and collect all the events flags
	for (std::list<ev_io*>::iterator it = afd->watcher_list_.begin(); it != afd->watcher_list_.end();
			++it) {
		//TODO
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
		} else if (EV_SUCCESS != (ret = polling(0.))) {
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
		//append to anfd wathcer list
		int fd = event_io->get_fd();
		anfd *afd = NULL;
		if (fdmap_.count(fd) == 0) {
			NEW(anfd, afd, sizeof(anfd));
			if (NULL != afd) {
				fdmap_.insert( { fd, afd });
			} else {
				ret = EV_ALLOC_FAILED;
			}
		}
		if (EV_SUCCESS == ret) {
			assert(NULL != afd);
			afd->watcher_list_.push_back(event_io);
			ret = fd_change(event_io->get_fd(), event_io->get_events());
		}
		TBSYS_LOG(INFO, "fd_change result fd=%d, ret=%d", fd, ret);
	}
	return ret;
}

int ev_loop::fd_change(int fd, int revents)
{
	//TODO
	int ret = EV_SUCCESS;
	fdchanges_.push(fd);
	return ret;
}

int ev_loop::fd_reify()
{
	int ret = EV_SUCCESS;
	while (!fdchanges_.empty() && EV_SUCCESS == ret) {
		int fd = fdchanges_.front();
		anfd *an_fd = NULL;
		if (fdmap_.count(fd) == 0) {
			NEW(anfd, an_fd, sizeof(anfd));
			if (NULL != an_fd) {
				fdmap_.insert( { fd, an_fd });
			} else {
				ret = EV_ALLOC_FAILED;
			}
		} else {
			an_fd = fdmap_.at(fd);
		}
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
		if (EV_SUCCESS == ret) {
			fdchanges_.pop();
		}
		TBSYS_LOG(INFO, "backend_modify epoll ctrl result is ret=%d, fd=%d", ret, fd);
	}
	return ret;
}

int ev_loop::backend_modify(int fd, unsigned char o_events, unsigned char cur_events)
{
	return epoll_modify(fd, o_events, cur_events);
}

int ev_loop::polling(double timeout)
{
	return epoll_poll(timeout);
}
int ev_loop::epoll_poll(double timeout)
{
	int ret = EV_SUCCESS;
	int i = 0;
	int eventcnt = 0;

	if ((epoll_epermcnt)) {
		timeout = 0.;
	}

	/* epoll wait times cannot be larger than (LONG_MAX - 999UL) / HZ msecs, which is below */
	/* the default libev max wait time, however. */
	//EV_RELEASE_CB;
	eventcnt = epoll_wait(ev_backend_fd_, epoll_events, epoll_eventmax, timeout * 1e3);
	//EV_ACQUIRE_CB;

	//TBSYS_LOG(INFO, "polling result eventcnt=%d", eventcnt);

	if ((eventcnt < 0)) {
		ret = EV_EPOLL_FAIL;
		if (errno != EINTR) {
			TBSYS_LOG(ERROR, "fatal error epoll error errno=%d", errno);
			abort();
		}
	} else {
		for (i = 0; EV_SUCCESS == ret && i < eventcnt; ++i) {
			struct epoll_event *ev = epoll_events + i;

			int fd = (uint32_t) ev->data.u64; /* mask out the lower 32 bits */
			anfd *afd = NULL;
			if (fdmap_.count(fd) == 0) {
				ret = EV_INVALID_ARGS;
				continue;
			} else {
				afd = fdmap_.at(fd);
			}
			int want = afd->events_;
			int got = (ev->events & (EPOLLOUT | EPOLLERR | EPOLLHUP) ? EV_WRITE : 0)
					| (ev->events & (EPOLLIN | EPOLLERR | EPOLLHUP) ? EV_READ : 0);

			/*
			 * check for spurious notification.
			 * this only finds spurious notifications on egen updates
			 * other spurious notifications will be found by epoll_ctl, below
			 * we assume that fd is always in range, as we never shrink the anfds array
			 */
			if (((uint32_t) afd->egen_ != (uint32_t) (ev->data.u64 >> 32))) {
				/* recreate kernel state */
				//postfork |= 2; ray
				continue;
			}

			if ((got & ~want)) {
				assert(0);
				afd->emask_ = want;

				/*
				 * we received an event but are not interested in it, try mod or del
				 * this often happens because we optimistically do not unregister fds
				 * when we are no longer interested in them, but also when we get spurious
				 * notifications for fds from another process. this is partially handled
				 * above with the gencounter check (== our fd is not the event fd), and
				 * partially here, when epoll_ctl returns an error (== a child has the fd
				 * but we closed it).
				 */
				ev->events = (want & EV_READ ? EPOLLIN : 0) | (want & EV_WRITE ? EPOLLOUT : 0);

				/* pre-2.6.9 kernels require a non-null pointer with EPOLL_CTL_DEL, */
				/* which is fortunately easy to do for us. */
				if (epoll_ctl(ev_backend_fd_, want ? EPOLL_CTL_MOD : EPOLL_CTL_DEL, fd, ev)) {
					//postfork |= 2; /* an error occurred, recreate kernel state */ TODO
					continue;
				}
			}
			ret = fd_event(afd, got);
			//TBSYS_LOG(INFO, "epoll result is ret=%d, fd=%d", ret, fd);
		}

		/* if the receive array was full, increase its size */
		if ((eventcnt == epoll_eventmax)) {
			tc_free(epoll_events);
			epoll_eventmax = array_nextsize(sizeof(struct epoll_event), epoll_eventmax,
					epoll_eventmax + 1);
			epoll_events =
					(struct epoll_event *) (tc_malloc(sizeof(struct epoll_event) * epoll_eventmax));
		}

		/* now synthesize events for all fds where epoll fails, while select works... */
		/* TODO check this later
		 for (i = epoll_epermcnt; i--;) {
		 int fd = epoll_eperms[i];
		 unsigned char events = anfds[fd].events & (EV_READ | EV_WRITE);
		 if (anfds [fd].emask & EV_EMASK_EPERM && events) {
		 fd_event (EV_A_ fd, events);
		 } else
		 epoll_eperms [i] = epoll_eperms [--epoll_epermcnt];
		 anfds [fd].emask = 0;
		 }
		 }
		 */
	}
	return ret;
}

/*
 * param:
 *      events: actually get events from epoll
 */
int ev_loop::fd_event(anfd *afd, int events)
{
	int ret = EV_SUCCESS;
	assert(NULL != afd);
	if (!afd->reify_) {
		ret = fd_event_no_check(afd, events);
	}
	return ret;
}

int ev_loop::fd_event_no_check(anfd *afd, int events)
{
	int ret = EV_SUCCESS;
	for (std::list<ev_io*>::iterator it = afd->watcher_list_.begin();
	EV_SUCCESS == ret && it != afd->watcher_list_.end(); ++it) {
		//TODO simple queue structure
		pendings_.push(process_info(afd, events));
	}
	return ret;
}

int ev_loop::invoke_pending()
{
	int ret = EV_SUCCESS;
	while (EV_SUCCESS == ret && !pendings_.empty()) {
		process_info proc = pendings_.front();
		for (std::list<ev_io*>::iterator it = proc.afd->watcher_list_.begin();
		EV_SUCCESS == ret && it != proc.afd->watcher_list_.end(); ++it) {
			ev_io *cur_watcher = *it;
			ret = cur_watcher->callback(proc.events);
		}
		pendings_.pop();
		//TBSYS_LOG(INFO, "pending processing done, ret=%d", ret);
	}
	return ret;
}
