#ifndef TEST_CSQ_STATIC_H
#define TEST_CSQ_STATIC_H

#include "AtCommand.h"

size_t CsqSerialize(void *obj, char *obuf, size_t length);
size_t CsqParse(void *obj, const char *ibuf, size_t length);
void CsqTimeout(void *p);

#endif