/*
 * EVBase.cpp
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#include "ev_watcher.h"
#include "common/common.h"

ev_watcher::~ev_watcher() {
  // TODO Auto-generated destructor stub
}

int ev_watcher::callback(int revents) {
  return nullptr != cb_ ? cb_(this, revents) : EV_INVALID_ARGS;
}
