#ifndef TEST_ATCFUN_STATIC_H
#define TEST_ATCFUN_STATIC_H

#include "AtCommand.h"

size_t AtCfunSerialize(void *obj, char *obuf, size_t length);
size_t AtCfunParse(void *obj, const char *ibuf, size_t length);
void AtCfunTimeout(void *p);

#endif