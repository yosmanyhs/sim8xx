#ifndef TEST_HTTPTERM_STATIC_H
#define TEST_HTTPTERM_STATIC_H

#include "AtCommand.h"

size_t HttptermSerialize(void *obj, char *obuf, size_t length);
size_t HttptermParse(void *obj, const char *ibuf, size_t length);
void HttptermTimeout(void *p);

#endif