#ifndef TEST_BTHOST_STATIC_H
#define TEST_BTHOST_STATIC_H

#include "AtCommand.h"

size_t BtHostSerialize(void *obj, char *obuf, size_t length);
size_t BtHostParse(void *obj, const char *ibuf, size_t length);

#endif