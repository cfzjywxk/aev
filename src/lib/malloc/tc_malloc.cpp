//
// Created by ray on 16-10-26.
//
#include <malloc.h>

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
