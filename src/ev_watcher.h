/*
 * EVBase.h
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#ifndef EVBASE_H_
#define EVBASE_H_
#include <boost/utility.hpp>

class ev_watcher;
typedef	int (*CallbackPtr)(ev_watcher *watcher, int revents);

class ev_watcher : public boost::noncopyable {
public:
	ev_watcher() : cb_(NULL) {};
	ev_watcher(CallbackPtr ptr) : cb_(ptr) {};
	virtual ~ev_watcher();

	void set_callback(CallbackPtr ptr) {cb_ = ptr;}
	virtual int callback(int revents);
protected:
	CallbackPtr cb_;
};

#endif /* EVBASE_H_ */
