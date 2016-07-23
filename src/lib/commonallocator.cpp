/*
 * commonallocator.cpp
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#include "commonallocator.h"
#include <stddef.h>
#include "lib/Common.h"

common_allocator::common_allocator() {
	// TODO Auto-generated constructor stub

}

common_allocator::~common_allocator() {
	// TODO Auto-generated destructor stub
}

int tc_malloc(size_t len, void *&obj)
{
	int ret = EV_SUCCESS;
	void *mem = malloc(len);
	if (NULL != mem) {
		obj = mem;
	} else {
		ret = EV_ALLOC_FAILED;
		obj = NULL;
	}
	return ret;
}
