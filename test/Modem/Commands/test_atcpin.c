#include "atcpin.h"
#include "test_atcpin_static.h"
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

void test_AtCpinObjectInit(void)
{
  AtCpin_t atcpin;
  AtCpinObjectInit(&atcpin);

  TEST_ASSERT_EQUAL(&atcpin, atcpin.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(AtCpinSerialize, atcpin.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(AtCpinParse, atcpin.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(AtCpinTimeout, atcpin.atcmd.timeout);
  TEST_ASSERT_EQUAL(5000, atcpin.atcmd.timeoutInMilliSec);
}

void test_AtGetAtCommand(void)
{
  AtCpin_t atcpin;
  AtCpinObjectInit(&atcpin);

  AT_Command_t *atcmd = AtCpinGetAtCommand(&atcpin);

  TEST_ASSERT_EQUAL(&atcpin, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(AtCpinSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(AtCpinParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(AtCpinTimeout, atcmd->timeout);
  TEST_ASSERT_EQUAL(5000, atcmd->timeoutInMilliSec);
}

void test_AtSerialize_NoPin(void)
{
  AtCpin_t atcpin;
  AtCpinObjectInit(&atcpin);

  char obuf[50] = {0};
  size_t n      = AtCpinSerialize(&atcpin, obuf, sizeof(obuf));

  const char *expected  = "";
  size_t expectedLength = 0;

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_AtSerialize_Pin(void)
{
  AtCpin_t atcpin;
  AtCpinObjectInit(&atcpin);
  AtCpinSetupRequest(&atcpin, "3943", NULL);

  char obuf[50] = {0};
  size_t n      = AtCpinSerialize(&atcpin, obuf, sizeof(obuf));

  const char *expected  = "AT+CPIN=3943\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_AtSerialize_PinAndPin2(void)
{
  AtCpin_t atcpin;
  AtCpinObjectInit(&atcpin);
  AtCpinSetupRequest(&atcpin, "3943", "1234");

  char obuf[50] = {0};
  size_t n      = AtCpinSerialize(&atcpin, obuf, sizeof(obuf));

  const char *expected  = "AT+CPIN=3943,1234\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_AtParse_OK(void)
{
  AtCpin_t atcpin;
  AtCpinObjectInit(&atcpin);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen      = strlen(ibuf);

  size_t n = AtCpinParse(&atcpin, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, atcpin.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AtTimeout(void)
{
  AtCpin_t atcpin;
  AtCpinObjectInit(&atcpin);

  AtCpinTimeout(&atcpin);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, atcpin.response.status);
}

void test_AtCpinIsURC(void)
{
  const char *ibuf = "\r\n+CPIN: READY\r\n";
  size_t ilen      = strlen(ibuf);

  bool result = AtCpinIsURC(ibuf, ilen);

  TEST_ASSERT(result);
}

void test_AtCpinParseURC(void)
{
  const char *ibuf = "\r\n+CPIN: READY\r\n";
  size_t ilen      = strlen(ibuf);

  AtCpinURC_t urc = {0};
  size_t n = AtCpinParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(ATCPIN_INFO, urc.type);
  TEST_ASSERT_EQUAL(ATCPIN_READY, urc.payload.info.code);
}