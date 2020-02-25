#include "cfun.h"
#include "test_cfun_static.h"
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

void test_CfunObjectInit(void)
{
  Cfun_t cfun;
  CfunObjectInit(&cfun);

  TEST_ASSERT_EQUAL(&cfun, cfun.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CfunSerialize, cfun.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CfunParse, cfun.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(CfunTimeout, cfun.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, cfun.atcmd.timeoutInMilliSec);
}

void test_AtGetAtCommand(void)
{
  Cfun_t cfun;
  CfunObjectInit(&cfun);

  AT_Command_t *atcmd = CfunGetAtCommand(&cfun);

  TEST_ASSERT_EQUAL(&cfun, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CfunSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CfunParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(CfunTimeout, atcmd->timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_AtSerialize(void)
{
  Cfun_t cfun;
  CfunObjectInit(&cfun);
  CfunSetupRequest(&cfun, 4, 1);

  char obuf[50] = {0};
  size_t n      = CfunSerialize(&cfun, obuf, sizeof(obuf));

  const char *expected  = "AT+CFUN=4,1\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_AtParse_OK(void)
{
  Cfun_t cfun;
  CfunObjectInit(&cfun);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen      = strlen(ibuf);

  size_t n = CfunParse(&cfun, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, cfun.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AtParse_CME_ERROR(void)
{
  Cfun_t cfun;
  CfunObjectInit(&cfun);

  const char *ibuf = "\r\n+CME ERROR: 24\r\n";
  size_t ilen      = strlen(ibuf);

  size_t n = CfunParse(&cfun, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, cfun.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AtParse_InvalidResponse(void)
{
  Cfun_t cfun;
  CfunObjectInit(&cfun);

  const char *ibuf = "\r\n+INVALID: 24\r\n";
  size_t ilen      = strlen(ibuf);

  size_t n = CfunParse(&cfun, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cfun.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_AtTimeout(void)
{
  Cfun_t cfun;
  CfunObjectInit(&cfun);

  CfunTimeout(&cfun);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, cfun.response.status);
}

void test_CfunIsURC(void)
{
  const char *ibuf = "\r\n+CFUN: 1\r\n";
  size_t ilen      = strlen(ibuf);

  bool result = CfunIsURC(ibuf, ilen);

  TEST_ASSERT(result);
}

void test_CfunParseURC_LEVEL_MINIMUM(void)
{
  const char *ibuf = "\r\n+CFUN: 1\r\n";
  size_t ilen      = strlen(ibuf);

  CfunURC_t urc = {0};
  size_t n        = CfunParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CFUN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CFUN_LEVEL_MINIMUM, urc.payload.info.code);
}

void test_CfunParseURC_LEVEL_FULL(void)
{
  const char *ibuf = "\r\n+CFUN: 2\r\n";
  size_t ilen      = strlen(ibuf);

  CfunURC_t urc = {0};
  size_t n        = CfunParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CFUN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CFUN_LEVEL_FULL, urc.payload.info.code);
}

void test_CfunParseURC_LEVEL_DISABLE_PHONE(void)
{
  const char *ibuf = "\r\n+CFUN: 4\r\n";
  size_t ilen      = strlen(ibuf);

  CfunURC_t urc = {0};
  size_t n        = CfunParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CFUN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CFUN_LEVEL_DISABLE_PHONE, urc.payload.info.code);
}

void test_CfunParseURC_LEVEL_INVALID(void)
{
  const char *ibuf = "\r\n+CFUN: 99\r\n";
  size_t ilen      = strlen(ibuf);

  CfunURC_t urc = {0};
  size_t n        = CfunParseURC(&urc, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(CFUN_INFO, urc.type);
  TEST_ASSERT_EQUAL(CFUN_LEVEL_INVALID, urc.payload.info.code);
}