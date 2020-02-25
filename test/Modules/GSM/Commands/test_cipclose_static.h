#ifndef TEST_CIPCLOSE_STATIC_H
#define TEST_CIPCLOSE_STATIC_H

#include "AtCommand.h"

size_t CipcloseSerialize(void *obj, char *obuf, size_t length);
size_t CipcloseParse(void *obj, const char *ibuf, size_t length);
void CipcloseTimeout(void *p);

#endif