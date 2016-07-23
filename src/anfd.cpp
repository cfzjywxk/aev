/*
 * anfd.cpp
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#include "anfd.h"

anfd::anfd() : watcher_list_(), events_(0), reify_(0), unused_(0), emask_(0){
	// TODO Auto-generated constructor stub

}

anfd::~anfd() {
	// TODO Auto-generated destructor stub
}

