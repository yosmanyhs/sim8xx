#ifndef TEST_BTACPT_STATIC_H
#define TEST_BTACPT_STATIC_H

#include "AtCommand.h"

size_t BtAcptSerialize(void *obj, char *obuf, size_t length);
size_t BtAcptParse(void *obj, const char *ibuf, size_t length);
void BtAcptTimeout(void *p);

#endif