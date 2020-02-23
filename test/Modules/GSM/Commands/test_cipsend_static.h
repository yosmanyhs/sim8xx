#ifndef TEST_CIPSEND_STATIC_H
#define TEST_CIPSEND_STATIC_H

#include "AtCommand.h"

size_t CipsendSerialize(void *obj, char *obuf, size_t length);
size_t CipsendParse(void *obj, const char *ibuf, size_t length);
void CipsendTimeout(void *p);

#endif