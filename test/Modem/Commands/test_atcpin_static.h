#ifndef TEST_ATCPIN_STATIC_H
#define TEST_ATCPIN_STATIC_H

#include "AtCommand.h"

size_t AtCpinSerialize(void *obj, char *obuf, size_t length);
size_t AtCpinParse(void *obj, const char *ibuf, size_t length);
void AtCpinTimeout(void *p);

#endif