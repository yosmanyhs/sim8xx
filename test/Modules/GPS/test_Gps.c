#include "unity.h"
#include "Gps.h"

#include "test_Gps_static.h"
#include "mock_Modem.h"
#include <string.h>


TEST_FILE("cgnspwr.c");
TEST_FILE("cgnsinf.c");
TEST_FILE("At.c");
TEST_FILE("Utils.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_GSM_GpsObjectInit(void)
{
  GSM_Modem_t modem;
  GSM_Gps_t gps;
  GSM_GpsObjectInit(&gps, &modem);

  TEST_ASSERT_EQUAL_PTR(&modem, gps.parent);
}

void test_GSM_gpsConvertRawDateTimeToGpsDateTime(void)
{
  char *ibuf = "20180925163521.000";
  size_t ilen = strlen(ibuf);

  GPS_Time_t time = {0};
  bool result = GSM_gpsConvertRawDateTimeToGpsDateTime(&time, ibuf, ilen);

  TEST_ASSERT(result);
  TEST_ASSERT_EQUAL(2018, time.year);
  TEST_ASSERT_EQUAL(9, time.month);
  TEST_ASSERT_EQUAL(25, time.day);
  TEST_ASSERT_EQUAL(16, time.hour);
  TEST_ASSERT_EQUAL(35, time.min);
  TEST_ASSERT_EQUAL(21, time.sec);
  TEST_ASSERT_EQUAL(0, time.msec);
}