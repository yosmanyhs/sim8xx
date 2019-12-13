#ifndef TEST_BTPAIRCFG_STATIC_H
#define TEST_BTPAIRCFG_STATIC_H

#include "AtCommand.h"

size_t BtPaircfgSerialize(void *obj, char *obuf, size_t length);
size_t BtPaircfgParse(void *obj, const char *ibuf, size_t length);

#endif