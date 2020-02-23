#ifndef TEST_CIICR_STATIC_H
#define TEST_CIICR_STATIC_H

#include "AtCommand.h"

size_t CiicrSerialize(void *obj, char *obuf, size_t length);
size_t CiicrParse(void *obj, const char *ibuf, size_t length);
void CiicrTimeout(void *p);

#endif