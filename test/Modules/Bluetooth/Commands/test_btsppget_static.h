#ifndef TEST_BTSPPGET_STATIC_H
#define TEST_BTSPPGET_STATIC_H

#include "At.h"

bool BtSppGetIsSppGet(const char *ibuf, size_t length);
size_t BtSppGet_parseSppData(BtSppGetDataURC_t *urc, const char *ibuf, size_t length);

#endif