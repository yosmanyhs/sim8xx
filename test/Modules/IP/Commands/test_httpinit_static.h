#ifndef TEST_HTTPINIT_STATIC_H
#define TEST_HTTPINIT_STATIC_H

#include "AtCommand.h"

size_t HttpinitSerialize(void *obj, char *obuf, size_t length);
size_t HttpinitParse(void *obj, const char *ibuf, size_t length);
void HttpinitTimeout(void *p);

#endif