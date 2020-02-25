#include "btsppurc.h"
#include "test_btsppurc_static.h"
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

void test_BtsppurcObjectInit(void)
{
  Btsppurc_t btsppurc;
  BtsppurcObjectInit(&btsppurc);

  TEST_ASSERT_EQUAL(&btsppurc, btsppurc.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(BtsppurcSerialize, btsppurc.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(BtsppurcParse, btsppurc.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(BtsppurcTimeout, btsppurc.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, btsppurc.atcmd.timeoutInMilliSec);
}

void test_BtsppurcGetAtCommand(void)
{
  Btsppurc_t btsppurc;
  BtsppurcObjectInit(&btsppurc);

  AT_Command_t *atcmd = BtsppurcGetAtCommand(&btsppurc);

  TEST_ASSERT_EQUAL(&btsppurc, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(BtsppurcSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(BtsppurcParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(BtsppurcTimeout, btsppurc.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_BtsppurcSerialize_Mode0(void)
{
  Btsppurc_t btsppurc;
  BtsppurcObjectInit(&btsppurc);
  BtsppurcSetupRequest(&btsppurc, 0);

  char obuf[32] = {0};
  size_t n      = BtsppurcSerialize(&btsppurc, obuf, sizeof(obuf));

  const char *expected = "AT+BTSPPURC=0\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_BtsppurcSerialize_Mode1(void)
{
  Btsppurc_t btsppurc;
  BtsppurcObjectInit(&btsppurc);
  BtsppurcSetupRequest(&btsppurc, 1);

  char obuf[32] = {0};
  size_t n      = BtsppurcSerialize(&btsppurc, obuf, sizeof(obuf));

  const char *expected = "AT+BTSPPURC=1\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_BtsppurcParse_OK(void)
{
  Btsppurc_t btsppurc;
  BtsppurcObjectInit(&btsppurc);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = BtsppurcParse(&btsppurc, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, btsppurc.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_BtsppurcParse_ERROR(void)
{
  Btsppurc_t btsppurc;
  BtsppurcObjectInit(&btsppurc);

  const char *ibuf = "\r\nERROR\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = BtsppurcParse(&btsppurc, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btsppurc.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_BtsppurcParse_Incomplete(void)
{
  Btsppurc_t btsppurc;
  BtsppurcObjectInit(&btsppurc);

  const char *ibuf = "\r\nOK";
  size_t length    = strlen(ibuf);

  size_t n = BtsppurcParse(&btsppurc, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btsppurc.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_BtsppurcParse_InvalidStatus(void)
{
  Btsppurc_t btsppurc;
  BtsppurcObjectInit(&btsppurc);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = BtsppurcParse(&btsppurc, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btsppurc.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_BtsppurcTimeout(void)
{
  Btsppurc_t btsppurc;
  BtsppurcObjectInit(&btsppurc);

  BtsppurcTimeout(&btsppurc);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, btsppurc.response.status);
}
