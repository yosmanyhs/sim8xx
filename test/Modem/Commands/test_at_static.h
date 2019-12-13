#ifndef TEST_AT_STATIC_H
#define TEST_AT_STATIC_H

#include "AtCommand.h"

size_t AtSerialize(void *obj, char *obuf, size_t length);
size_t AtParse(void *obj, const char *ibuf, size_t length);

#endif