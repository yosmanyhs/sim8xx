#include "cpin.h"
#include "test_cpin_static.h"
#include "unity.h"

#include <stdio.h>
#include <string.h>


TEST_FILE("AtCommand.c");
TEST_FILE("Utils.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_CpinObjectInit(void)
{
  Cpin_t cpin;
  CpinObjectInit(&cpin);

  TEST_ASSERT_EQUAL(&cpin, cpin.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CpinSerialize, cpin.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CpinParse, cpin.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(CpinTimeout, cpin.atcmd.timeout);
  TEST_ASSERT_EQUAL(5000, cpin.atcmd.timeoutInMilliSec);
}

void test_AtGetAtCommand(void)
{
  Cpin_t cpin;
  CpinObjectInit(&cpin);

  AT_Command_t *atcmd = CpinGetAtCommand(&cpin);

  TEST_ASSERT_EQUAL(&cpin, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CpinSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CpinParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(CpinTimeout, atcmd->timeout);
  TEST_ASSERT_EQUAL(5000, atcmd->timeoutInMilliSec);
}

void test_AtSerialize_NoPin(void)
{
  Cpin_t cpin;
  CpinObjectInit(&cpin);

  char obuf[50] = {0};
  size_t n      = CpinSerialize(&cpin, obuf, sizeof(obuf));

  const char *expected  = "";
  size_t expectedLength = 0;

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_AtSerialize_Pin(void)
{
  Cpin_t cpin;
  CpinObjectInit(&cpin);
  CpinSetupRequest(&cpin, "3943", NULL);

  char obuf[50] = {0};
  size_t n      = CpinSerialize(&cpin, obuf, sizeof(obuf));

  const char *expected  = "AT+CPIN=3943\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_AtSerialize_PinAndPin2(void)
{
  Cpin_t cpin;
  CpinObjectInit(&cpin);
  CpinSetupRequest(&cpin, "3943", "1234");

  char obuf[50] = {0};
  size_t n      = CpinSerialize(&cpin, obuf, sizeof(obuf));

  const char *expected  = "AT+CPIN=3943,1234\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_AtParse_OK(void)
{
  Cpin_t cpin;
  CpinObjectInit(&cpin);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen      = strlen(ibuf);

  size_t n = CpinParse(&cpin, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, cpin.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AtParse_CME_ERROR(void)
{
  Cpin_t cpin;
  CpinObjectInit(&cpin);

  const char *ibuf = "\r\n+CME ERROR: 24\r\n";
  size_t ilen      = strlen(ibuf);

  size_t n = CpinParse(&cpin, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, cpin.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AtParse_InvalidResponse(void)
{
  Cpin_t cpin;
  CpinObjectInit(&cpin);

  const char *ibuf = "\r\n+INVALID: 24\r\n";
  size_t ilen      = strlen(ibuf);

  size_t n = CpinParse(&cpin, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cpin.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_AtTimeout(void)
{
  Cpin_t cpin;
  CpinObjectInit(&cpin);

  CpinTimeout(&cpin);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, cpin.response.status);
}

void test_CpinIsURC(void)
{
  const char *ibuf = "\r\n+CPIN: READY\r\n";
  size_t ilen      = strlen(ibuf);

  bool result = CpinIsURC(ibuf, ilen);

  TEST_ASSERT(result);
}

void test_CpinParseURC_NOT_INSERTED(void)
{
  const char *ibuf = "\r\n+CPIN: NOT INSERTED\r\n";
  size_t ilen      = strlen(ibuf);

  CpinURC_t urc = {0};
  size_t n = CpinParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CPIN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CPIN_NOT_INSERTED, urc.payload.info.code);
}

void test_CpinParseURC_READY(void)
{
  const char *ibuf = "\r\n+CPIN: READY\r\n";
  size_t ilen      = strlen(ibuf);

  CpinURC_t urc = {0};
  size_t n = CpinParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CPIN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CPIN_READY, urc.payload.info.code);
}

void test_CpinParseURC_SIM_PIN(void)
{
  const char *ibuf = "\r\n+CPIN: SIM PIN\r\n";
  size_t ilen      = strlen(ibuf);

  CpinURC_t urc = {0};
  size_t n = CpinParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CPIN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CPIN_SIM_PIN, urc.payload.info.code);
}

void test_CpinParseURC_SIM_PUK(void)
{
  const char *ibuf = "\r\n+CPIN: SIM PUK\r\n";
  size_t ilen      = strlen(ibuf);

  CpinURC_t urc = {0};
  size_t n = CpinParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CPIN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CPIN_SIM_PUK, urc.payload.info.code);
}

void test_CpinParseURC_PH_SIM_PIN(void)
{
  const char *ibuf = "\r\n+CPIN: PH_SIM PIN\r\n";
  size_t ilen      = strlen(ibuf);

  CpinURC_t urc = {0};
  size_t n = CpinParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CPIN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CPIN_PH_SIM_PIN, urc.payload.info.code);
}

void test_CpinParseURC_PH_SIM_PUK(void)
{
  const char *ibuf = "\r\n+CPIN: PH_SIM PUK\r\n";
  size_t ilen      = strlen(ibuf);

  CpinURC_t urc = {0};
  size_t n = CpinParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CPIN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CPIN_PH_SIM_PUK, urc.payload.info.code);
}

void test_CpinParseURC_SIM_PIN2(void)
{
  const char *ibuf = "\r\n+CPIN: SIM PIN2\r\n";
  size_t ilen      = strlen(ibuf);

  CpinURC_t urc = {0};
  size_t n = CpinParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CPIN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CPIN_SIM_PIN2, urc.payload.info.code);
}

void test_CpinParseURC_SIM_PUK2(void)
{
  const char *ibuf = "\r\n+CPIN: SIM PUK2\r\n";
  size_t ilen      = strlen(ibuf);

  CpinURC_t urc = {0};
  size_t n = CpinParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CPIN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CPIN_SIM_PUK2, urc.payload.info.code);
}