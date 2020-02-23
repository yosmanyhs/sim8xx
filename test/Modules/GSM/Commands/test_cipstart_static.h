#ifndef TEST_CIPSTART_STATIC_H
#define TEST_CIPSTART_STATIC_H

#include "AtCommand.h"

size_t CipstartSerialize(void *obj, char *obuf, size_t length);
size_t CipstartParse(void *obj, const char *ibuf, size_t length);
void CipstartTimeout(void *p);

#endif