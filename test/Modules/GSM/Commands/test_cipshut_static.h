#ifndef TEST_CIPSHUT_STATIC_H
#define TEST_CIPSHUT_STATIC_H

#include "AtCommand.h"

size_t CipshutSerialize(void *obj, char *obuf, size_t length);
size_t CipshutParse(void *obj, const char *ibuf, size_t length);
void CipshutTimeout(void *p);

#endif