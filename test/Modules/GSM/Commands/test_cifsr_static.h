#ifndef TEST_CIFSR_STATIC_H
#define TEST_CIFSR_STATIC_H

#include "AtCommand.h"

size_t CifsrSerialize(void *obj, char *obuf, size_t length);
size_t CifsrParse(void *obj, const char *ibuf, size_t length);
void CifsrTimeout(void *p);

#endif