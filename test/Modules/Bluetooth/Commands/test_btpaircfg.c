#include "btpaircfg.h"
#include "test_btpaircfg_static.h"
#include "unity.h"

#include <string.h>

TEST_FILE("AtCommand.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_BtPaircfgObjectInit(void)
{
  BtPaircfg_t btpaircfg;
  BtPaircfgObjectInit(&btpaircfg);

  TEST_ASSERT_EQUAL(&btpaircfg, btpaircfg.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(BtPaircfgSerialize, btpaircfg.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(BtPaircfgParse, btpaircfg.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(BtPaircfgTimeout, btpaircfg.atcmd.timeout);
  TEST_ASSERT_EQUAL(2000, btpaircfg.atcmd.timeoutInMilliSec);
}

void test_BtPaircfgSetupRequest_Mode0(void)
{
  BtPaircfg_t btpaircfg;
  BtPaircfgObjectInit(&btpaircfg);
  BtPaircfgSetupRequest(&btpaircfg, 0, NULL);

  TEST_ASSERT_EQUAL(0, btpaircfg.request.mode);
  TEST_ASSERT_EQUAL_PTR(NULL, btpaircfg.request.pin);
}

void test_BtPaircfgSetupRequest_Mode1(void)
{
  BtPaircfg_t btpaircfg;
  BtPaircfgObjectInit(&btpaircfg);
  const char *pin = "2019";
  BtPaircfgSetupRequest(&btpaircfg, 1, pin);

  TEST_ASSERT_EQUAL(1, btpaircfg.request.mode);
  TEST_ASSERT_EQUAL_PTR(pin, btpaircfg.request.pin);
}

void test_BtPaircfgSetupRequest_Mode2(void)
{
  BtPaircfg_t btpaircfg;
  BtPaircfgObjectInit(&btpaircfg);
  BtPaircfgSetupRequest(&btpaircfg, 2, NULL);

  TEST_ASSERT_EQUAL(2, btpaircfg.request.mode);
  TEST_ASSERT_EQUAL_PTR(NULL, btpaircfg.request.pin);
}

void test_BtPaircfgSerialize_Mode0(void)
{
  BtPaircfg_t btpaircfg;
  BtPaircfgObjectInit(&btpaircfg);
  BtPaircfgSetupRequest(&btpaircfg, 0, NULL);

  char obuf[32] = {0};
  size_t n      = BtPaircfgSerialize(&btpaircfg, obuf, sizeof(obuf));

  const char *expected = "AT+BTPAIRCFG=0\r";
  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(strlen(expected), n);
}

void test_BtPaircfgSerialize_Mode1(void)
{
  BtPaircfg_t btpaircfg;
  BtPaircfgObjectInit(&btpaircfg);
  const char *pin = "2019";
  BtPaircfgSetupRequest(&btpaircfg, 1, pin);

  char obuf[32] = {0};
  size_t n      = BtPaircfgSerialize(&btpaircfg, obuf, sizeof(obuf));

  const char *expected = "AT+BTPAIRCFG=1,2019\r";
  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(strlen(expected), n);
}

void test_BtPaircfgSerialize_Mode2(void)
{
  BtPaircfg_t btpaircfg;
  BtPaircfgObjectInit(&btpaircfg);
  BtPaircfgSetupRequest(&btpaircfg, 2, NULL);

  char obuf[32] = {0};
  size_t n      = BtPaircfgSerialize(&btpaircfg, obuf, sizeof(obuf));

  const char *expected = "AT+BTPAIRCFG=2\r";
  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(strlen(expected), n);
}

void test_BtPaircfgParse(void)
{
  BtPaircfg_t btpaircfg;
  BtPaircfgObjectInit(&btpaircfg);

  const char *ibuf = "\r\nOK\r\n=CGNSINF...";
  size_t length    = strlen(ibuf);

  size_t n = BtPaircfgParse(&btpaircfg, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_OK, btpaircfg.response.status);
  TEST_ASSERT_EQUAL(6, n);
}

void test_BtPaircfgParse_Incomplete(void)
{
  BtPaircfg_t btpaircfg;
  BtPaircfgObjectInit(&btpaircfg);

  const char *ibuf = "\r\nOK\r";
  size_t length    = strlen(ibuf);

  size_t n = BtPaircfgParse(&btpaircfg, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btpaircfg.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_BtPaircfgParse_InvalidStatus(void)
{
  BtPaircfg_t btpaircfg;
  BtPaircfgObjectInit(&btpaircfg);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = BtPaircfgParse(&btpaircfg, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btpaircfg.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_BtPaircfgTimeout(void)
{
  BtPaircfg_t btpaircfg;
  BtPaircfgObjectInit(&btpaircfg);

  BtPaircfgTimeout(&btpaircfg);

  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, btpaircfg.response.status);
}