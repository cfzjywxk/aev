/*
 * EVBase.cpp
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#include "ev_watcher.h"
#include "lib/Common.h"

ev_watcher::~ev_watcher()
{
	// TODO Auto-generated destructor stub
}

int ev_watcher::callback(int revents)
{
	return NULL != cb_ ? cb_(this, revents) : EV_INVALID_ARGS;
}
