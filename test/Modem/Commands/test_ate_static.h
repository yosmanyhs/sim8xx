#ifndef TEST_ATE_STATIC_H
#define TEST_ATE_STATIC_H

#include "AtCommand.h"

size_t AteSerialize(void *obj, char *obuf, size_t length);
size_t AteParse(void *obj, const char *ibuf, size_t length);
void AteTimeout(void *p);

#endif