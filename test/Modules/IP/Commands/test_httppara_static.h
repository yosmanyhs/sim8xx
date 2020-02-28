#ifndef TEST_HTTPPARA_STATIC_H
#define TEST_HTTPPARA_STATIC_H

#include "AtCommand.h"

size_t HttpparaSerialize(void *obj, char *obuf, size_t length);
size_t HttpparaParse(void *obj, const char *ibuf, size_t length);
void HttpparaTimeout(void *p);

#endif