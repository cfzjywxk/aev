/*
 * commonallocator.cpp
 *
 *  Created on: 2016年7月23日
 *      Author: ray
 */

#include "commonallocator.h"
#include <stddef.h>
#include <malloc.h>
#include "lib/Common.h"

common_allocator::common_allocator() {
	// TODO Auto-generated constructor stub

}

common_allocator::~common_allocator() {
	// TODO Auto-generated destructor stub
}

void *tc_malloc(int len)
{
	return (void *)malloc(len);
}

void tc_free(void *ptr)
{
	if (NULL != ptr) {
		free(ptr);
		ptr = NULL;
	}
}
