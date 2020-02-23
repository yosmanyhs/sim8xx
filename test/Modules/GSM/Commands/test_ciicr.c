#include "ciicr.h"
#include "test_ciicr_static.h"
#include "unity.h"

#include <string.h>
#include <stdio.h>

TEST_FILE("AtCommand.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_AtObjectInit(void)
{
  Ciicr_t ciicr;
  CiicrObjectInit(&ciicr);

  TEST_ASSERT_EQUAL(&ciicr, ciicr.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CiicrSerialize, ciicr.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CiicrParse, ciicr.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(CiicrTimeout, ciicr.atcmd.timeout);
  TEST_ASSERT_EQUAL(85000, ciicr.atcmd.timeoutInMilliSec);
}

void test_CiicrGetAtCommand(void)
{
  Ciicr_t ciicr;
  CiicrObjectInit(&ciicr);

  AT_Command_t *atcmd = CiicrGetAtCommand(&ciicr);

  TEST_ASSERT_EQUAL(&ciicr, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CiicrSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CiicrParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(CiicrTimeout, ciicr.atcmd.timeout);
  TEST_ASSERT_EQUAL(85000, atcmd->timeoutInMilliSec);
}

void test_CiicrSerialize(void)
{
  Ciicr_t ciicr;
  CiicrObjectInit(&ciicr);

  char obuf[32] = {0};
  size_t n      = CiicrSerialize(&ciicr, obuf, sizeof(obuf));

  const char *expected = "AT+CIICR\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_CiicrParse_OK(void)
{
  Ciicr_t ciicr;
  CiicrObjectInit(&ciicr);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CiicrParse(&ciicr, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, ciicr.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CiicrParse_Incomplete(void)
{
  Ciicr_t ciicr;
  CiicrObjectInit(&ciicr);

  const char *ibuf = "\r\nOK\r";
  size_t length    = strlen(ibuf);

  size_t n = CiicrParse(&ciicr, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, ciicr.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CiicrParse_InvalidStatus(void)
{
  Ciicr_t ciicr;
  CiicrObjectInit(&ciicr);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CiicrParse(&ciicr, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, ciicr.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CiicrTimeout(void)
{
  Ciicr_t ciicr;
  CiicrObjectInit(&ciicr);

  CiicrTimeout(&ciicr);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, ciicr.response.status);
}
