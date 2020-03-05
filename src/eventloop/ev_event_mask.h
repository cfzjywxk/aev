/*
 * ev_event_mask.h
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#ifndef EV_EVENT_MASK_H_
#define EV_EVENT_MASK_H_

/* eventmask, revents, events... */
enum {
  EV_UNDEF = (int) 0xFFFFFFFF, /* guaranteed to be invalid */
  EV_NONE = 0x00, /* no events */
  EV_READ = 0x01, /* ev_io detected read will not block */
  EV_WRITE = 0x02, /* ev_io detected write will not block */
  EV__IOFDSET = 0x80, /* internal use only */
  EV_IO = EV_READ, /* alias for type-detection */
  EV_TIMER = 0x00000100, /* timer timed out */
#if EV_COMPAT3
  EV_TIMEOUT = EV_TIMER, /* pre 4.0 API compatibility */
#endif
  EV_PERIODIC = 0x00000200, /* periodic timer timed out */
  EV_SIGNAL = 0x00000400, /* signal was received */
  EV_CHILD = 0x00000800, /* child/pid had status change */
  EV_STAT = 0x00001000, /* stat data changed */
  EV_IDLE = 0x00002000, /* event loop is idling */
  EV_PREPARE = 0x00004000, /* event loop about to poll */
  EV_CHECK = 0x00008000, /* event loop finished poll */
  EV_EMBED = 0x00010000, /* embedded event loop needs sweep */
  EV_FORK = 0x00020000, /* event loop resumed in child */
  EV_CLEANUP = 0x00040000, /* event loop resumed in child */
  EV_ASYNC = 0x00080000, /* async intra-loop signal */
  EV_CUSTOM = 0x01000000, /* for use by user code */
  EV_ERROR = (int) 0x80000000 /* sent when an error occurs */
};

#endif /* EV_EVENT_MASK_H_ */
