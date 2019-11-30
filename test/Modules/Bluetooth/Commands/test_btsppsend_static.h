#ifndef TEST_BTSPPSEND_STATIC_H
#define TEST_BTSPPSEND_STATIC_H

#include "At.h"

size_t BtSppSendSerialize(void *obj, char *obuf, size_t length);
size_t BtSppSendParse(void *obj, const char *ibuf, size_t length);

#endif