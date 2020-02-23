#include "cifsr.h"
#include "test_cifsr_static.h"
#include "unity.h"

#include <string.h>
#include <stdio.h>

TEST_FILE("AtCommand.c");
TEST_FILE("Utils.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_CifsrObjectInit(void)
{
  Cifsr_t cifsr;
  CifsrObjectInit(&cifsr);

  TEST_ASSERT_EQUAL(&cifsr, cifsr.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CifsrSerialize, cifsr.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CifsrParse, cifsr.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(CifsrTimeout, cifsr.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, cifsr.atcmd.timeoutInMilliSec);
}

void test_CifsrGetAtCommand(void)
{
  Cifsr_t cifsr;
  CifsrObjectInit(&cifsr);

  AT_Command_t *atcmd = CifsrGetAtCommand(&cifsr);

  TEST_ASSERT_EQUAL(&cifsr, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CifsrSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CifsrParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(CifsrTimeout, cifsr.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_CifsrSerialize(void)
{
  Cifsr_t cifsr;
  CifsrObjectInit(&cifsr);

  char obuf[32] = {0};
  size_t n      = CifsrSerialize(&cifsr, obuf, sizeof(obuf));

  const char *expected = "AT+CIFSR\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_CifsrParse(void)
{
  Cifsr_t cifsr;
  CifsrObjectInit(&cifsr);

  const char *ibuf = "\r\n101.231.245.128\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CifsrParse(&cifsr, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(AT_CMD_OK, cifsr.response.status);
  TEST_ASSERT_EQUAL_STRING("101.231.245.128", cifsr.response.ipaddr);
}

void test_CifsrParse_ERROR(void)
{
  Cifsr_t cifsr;
  CifsrObjectInit(&cifsr);

  const char *ibuf = "\r\nERROR\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CifsrParse(&cifsr, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, cifsr.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CifsrParse_Incomplete(void)
{
  Cifsr_t cifsr;
  CifsrObjectInit(&cifsr);

  const char *ibuf = "\r\n101.231.2";
  size_t length    = strlen(ibuf);

  size_t n = CifsrParse(&cifsr, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cifsr.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CifsrTimeout(void)
{
  Cifsr_t cifsr;
  CifsrObjectInit(&cifsr);

  CifsrTimeout(&cifsr);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, cifsr.response.status);
}
