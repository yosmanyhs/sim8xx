#include "Gps.h"
#include "unity.h"
#include "mock_Modem.h"
#include <string.h>


TEST_FILE("cgnspwr.c");
TEST_FILE("At.c");

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