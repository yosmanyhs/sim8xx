#ifndef TEST_CSTT_STATIC_H
#define TEST_CSTT_STATIC_H

#include "AtCommand.h"

size_t CsttSerialize(void *obj, char *obuf, size_t length);
size_t CsttParse(void *obj, const char *ibuf, size_t length);
void CsttTimeout(void *p);

#endif