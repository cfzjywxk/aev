/*
 * commonallocator.h
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#ifndef COMMONALLOCATOR_H_
#define COMMONALLOCATOR_H_

int tc_malloc(size_t len, void *obj);

#define NEW(type, obj, size) \
	do { \
	void *mem = NULL; \
	if (0 == tc_malloc(size, mem)) { \
		obj = new(mem) type(); \
  } else { \
  	obj = NULL;\
  };\
  } while(0)



class common_allocator {
public:
	common_allocator();
	virtual ~common_allocator();
};

#endif /* COMMONALLOCATOR_H_ */
