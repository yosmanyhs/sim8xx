#include "atcfun.h"
#include "test_atcfun_static.h"
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

void test_AtCfunObjectInit(void)
{
  AtCfun_t atcfun;
  AtCfunObjectInit(&atcfun);

  TEST_ASSERT_EQUAL(&atcfun, atcfun.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(AtCfunSerialize, atcfun.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(AtCfunParse, atcfun.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(AtCfunTimeout, atcfun.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcfun.atcmd.timeoutInMilliSec);
}

void test_AtGetAtCommand(void)
{
  AtCfun_t atcfun;
  AtCfunObjectInit(&atcfun);

  AT_Command_t *atcmd = AtCfunGetAtCommand(&atcfun);

  TEST_ASSERT_EQUAL(&atcfun, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(AtCfunSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(AtCfunParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(AtCfunTimeout, atcmd->timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_AtSerialize(void)
{
  AtCfun_t atcfun;
  AtCfunObjectInit(&atcfun);
  AtCfunSetupRequest(&atcfun, 4, 1);

  char obuf[50] = {0};
  size_t n      = AtCfunSerialize(&atcfun, obuf, sizeof(obuf));

  const char *expected  = "AT+CFUN=4,1\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_AtParse_OK(void)
{
  AtCfun_t atcfun;
  AtCfunObjectInit(&atcfun);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen      = strlen(ibuf);

  size_t n = AtCfunParse(&atcfun, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, atcfun.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AtParse_CME_ERROR(void)
{
  AtCfun_t atcfun;
  AtCfunObjectInit(&atcfun);

  const char *ibuf = "\r\n+CME ERROR: 24\r\n";
  size_t ilen      = strlen(ibuf);

  size_t n = AtCfunParse(&atcfun, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, atcfun.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AtParse_InvalidResponse(void)
{
  AtCfun_t atcfun;
  AtCfunObjectInit(&atcfun);

  const char *ibuf = "\r\n+INVALID: 24\r\n";
  size_t ilen      = strlen(ibuf);

  size_t n = AtCfunParse(&atcfun, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, atcfun.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_AtTimeout(void)
{
  AtCfun_t atcfun;
  AtCfunObjectInit(&atcfun);

  AtCfunTimeout(&atcfun);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, atcfun.response.status);
}

void test_AtCfunIsURC(void)
{
  const char *ibuf = "\r\n+CFUN: 1\r\n";
  size_t ilen      = strlen(ibuf);

  bool result = AtCfunIsURC(ibuf, ilen);

  TEST_ASSERT(result);
}

void test_AtCfunParseURC_LEVEL_MINIMUM(void)
{
  const char *ibuf = "\r\n+CFUN: 1\r\n";
  size_t ilen      = strlen(ibuf);

  AtCfunURC_t urc = {0};
  size_t n        = AtCfunParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(ATCFUN_INFO, urc.type);
  TEST_ASSERT_EQUAL(ATCFUN_LEVEL_MINIMUM, urc.payload.info.code);
}

void test_AtCfunParseURC_LEVEL_FULL(void)
{
  const char *ibuf = "\r\n+CFUN: 2\r\n";
  size_t ilen      = strlen(ibuf);

  AtCfunURC_t urc = {0};
  size_t n        = AtCfunParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(ATCFUN_INFO, urc.type);
  TEST_ASSERT_EQUAL(ATCFUN_LEVEL_FULL, urc.payload.info.code);
}

void test_AtCfunParseURC_LEVEL_DISABLE_PHONE(void)
{
  const char *ibuf = "\r\n+CFUN: 4\r\n";
  size_t ilen      = strlen(ibuf);

  AtCfunURC_t urc = {0};
  size_t n        = AtCfunParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(ATCFUN_INFO, urc.type);
  TEST_ASSERT_EQUAL(ATCFUN_LEVEL_DISABLE_PHONE, urc.payload.info.code);
}

void test_AtCfunParseURC_LEVEL_INVALID(void)
{
  const char *ibuf = "\r\n+CFUN: 99\r\n";
  size_t ilen      = strlen(ibuf);

  AtCfunURC_t urc = {0};
  size_t n        = AtCfunParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(ATCFUN_INFO, urc.type);
  TEST_ASSERT_EQUAL(ATCFUN_LEVEL_INVALID, urc.payload.info.code);
}