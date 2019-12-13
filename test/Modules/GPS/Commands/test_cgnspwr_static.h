#ifndef TEST_CGNSPWR_STATIC_H
#define TEST_CGNSPWR_STATIC_H

#include "AtCommand.h"

size_t CgnsPwrSerialize(void *obj, char *obuf, size_t length);
size_t CgnsPwrParse(void *obj, const char *ibuf, size_t length);

#endif