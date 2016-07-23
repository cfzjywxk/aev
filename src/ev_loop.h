/*
 * ev_loop.h
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#ifndef EV_LOOP_H_
#define EV_LOOP_H_
#include <boost/utility.hpp>
#include <vector>
#include <unordered_map>

class ev_io;
class anfd;
/*
 * one loop per thread
 */
class ev_loop : public boost::noncopyable {
public:
	ev_loop();
	virtual ~ev_loop();

	virtual int init_loop();
	virtual int run();
	int ev_io_start(ev_io *event_io);

protected:
	int fd_reify();
	int polling();
	int invoke_pending();
	int fd_change(int fd, int revents);
	int backend_modify(int fd, unsigned char o_events, unsigned char cur_events);

	//backend epoll
	int epoll_backend(int &backended_fd, bool &res);
	int epoll_modify(int fd, int old_ev, int new_ev);
	int epoll_poll();
	int fd_kill(anfd *afd);


	int ev_backend_fd_;
	bool ev_backended_;

	//epoll related
	double backend_mintime;
	struct epoll_event *epoll_events;
	int epoll_eventmax;

	double ev_timestamp_;
	bool loop_done_;
	std::vector<int> fdchanges_; //fd that changes by ev_io_start function call
	std::unordered_map<int, anfd*> fdmap_; //hash table fd -> anfd structure
};

#endif /* EV_LOOP_H_ */
