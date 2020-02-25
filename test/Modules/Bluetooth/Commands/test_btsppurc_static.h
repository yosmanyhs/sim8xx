#ifndef TEST_BTSPPURC_STATIC_H
#define TEST_BTSPPURC_STATIC_H

#include "AtCommand.h"

size_t BtsppurcSerialize(void *obj, char *obuf, size_t length);
size_t BtsppurcParse(void *obj, const char *ibuf, size_t length);
void BtsppurcTimeout(void *p);

#endif