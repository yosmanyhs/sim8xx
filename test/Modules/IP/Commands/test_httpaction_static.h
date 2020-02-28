#ifndef TEST_HTTPACTION_STATIC_H
#define TEST_HTTPACTION_STATIC_H

#include "AtCommand.h"

size_t HttpactionSerialize(void *obj, char *obuf, size_t length);
size_t HttpactionParse(void *obj, const char *ibuf, size_t length);
void HttpactionTimeout(void *p);

#endif