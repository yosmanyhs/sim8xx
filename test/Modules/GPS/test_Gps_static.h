#ifndef TEST_GPS_STATIC_H
#define TEST_GPS_STATIC_H

#include "At.h"

bool GSM_gpsConvertRawDateTimeToGpsDateTime(GPS_Time_t *time, char *ibuf, size_t ilen);

#endif