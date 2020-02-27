#ifndef TEST_SAPBR_STATIC_H
#define TEST_SAPBR_STATIC_H

#include "AtCommand.h"

size_t SapbrSerialize(void *obj, char *obuf, size_t length);
size_t SapbrParse(void *obj, const char *ibuf, size_t length);
void SapbrTimeout(void *p);

#endif