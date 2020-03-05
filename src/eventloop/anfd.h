/*
 * anfd.h
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#ifndef ANFD_H_
#define ANFD_H_

#include <list>

class ev_io;

class anfd {
public:
  anfd();

  virtual ~anfd();

public:
  std::list<ev_io *> watcher_list_;
  unsigned char events_; /* the events watched for */
  unsigned char reify_; /* flag set when this ANFD needs reification (EV_ANFD_REIFY, EV__IOFDSET) */
  unsigned char emask_; /* the epoll backend stores the actual kernel mask in here */
  unsigned char unused_;
  unsigned int egen_; /* generation counter to counter epoll bugs */
};

#endif /* ANFD_H_ */
