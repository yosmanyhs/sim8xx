#ifndef TEST_BTDISCONN_STATIC_H
#define TEST_BTDISCONN_STATIC_H

#include "AtCommand.h"

bool BtDisconnIsBtDisconn(const char *ibuf, size_t length);
size_t BtDisconn_parseBtDisconn(BtDisconnURC_t *urc, const char *ibuf, size_t length);

#endif