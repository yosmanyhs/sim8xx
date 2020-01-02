#ifndef TEST_BTCONNECT_STATIC_H
#define TEST_BTCONNECT_STATIC_H

#include "AtCommand.h"

size_t BtConnectSerialize(void *obj, char *obuf, size_t length);
size_t BtConnectParse(void *obj, const char *ibuf, size_t length);
void BtConnectTimeout(void *p);

#endif