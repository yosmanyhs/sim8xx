#ifndef TEST_CPIN_STATIC_H
#define TEST_CPIN_STATIC_H

#include "AtCommand.h"

size_t CpinSerialize(void *obj, char *obuf, size_t length);
size_t CpinParse(void *obj, const char *ibuf, size_t length);
void CpinTimeout(void *p);

#endif