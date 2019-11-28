#ifndef TEST_BTPOWER_STATIC_H
#define TEST_BTPOWER_STATIC_H

#include "at.h"

size_t BtPowerSerialize(void *obj, char *obuf, size_t length);
size_t BtPowerParse(void *obj, const char *ibuf, size_t length);

#endif