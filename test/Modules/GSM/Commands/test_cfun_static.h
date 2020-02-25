#ifndef TEST_CFUN_STATIC_H
#define TEST_CFUN_STATIC_H

#include "AtCommand.h"

size_t CfunSerialize(void *obj, char *obuf, size_t length);
size_t CfunParse(void *obj, const char *ibuf, size_t length);
void CfunTimeout(void *p);

#endif