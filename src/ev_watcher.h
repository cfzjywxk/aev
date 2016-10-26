/*
 * EVBase.h
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#ifndef EVBASE_H_
#define EVBASE_H_

#include "lib/def.h"

class ev_watcher;
typedef int (*CallbackPtr)(ev_watcher *watcher, int revents);

class ev_watcher {
public:
	ev_watcher() :
			cb_(nullptr)
	{
	}
	;
	ev_watcher(CallbackPtr ptr) :
			cb_(ptr)
	{
	}
	;
	virtual ~ev_watcher();

	void set_callback(CallbackPtr ptr)
	{
		cb_ = ptr;
	}
	virtual int callback(int revents);
protected:
	DISALLOW_COPY_AND_ASSIGN(ev_watcher);

	CallbackPtr cb_;
};

#endif /* EVBASE_H_ */
