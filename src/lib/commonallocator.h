/*
 * commonallocator.h
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#ifndef COMMONALLOCATOR_H_
#define COMMONALLOCATOR_H_

void *tc_malloc(size_t len);
void tc_free(void *ptr);

#define NEW(type, obj, size) \
	do { \
		void *mem = (void *)tc_malloc(size); \
		if (NULL != mem) { \
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

/* find a suitable new size for the given array, */
/* hopefully by rounding to a nice-to-malloc size */
#define MALLOC_ROUND 4096 /* prefer to allocate in chunks of this size, must be 2**n and >> 4 longs */
inline int array_nextsize (int elem, int cur, int cnt)
{
  int ncur = cur + 1;
  do
    ncur <<= 1;
  while (cnt > ncur);

  /* if size is large, round to MALLOC_ROUND - 4 * longs to accommodate malloc overhead */
	if (elem * ncur > MALLOC_ROUND - sizeof(void *) * 4) {
		ncur *= elem;
		ncur = (ncur + elem + (MALLOC_ROUND - 1) + sizeof(void *) * 4)
				& ~(MALLOC_ROUND - 1);
		ncur = ncur - sizeof(void *) * 4;
		ncur /= elem;
	}
  return ncur;
}

#endif /* COMMONALLOCATOR_H_ */
