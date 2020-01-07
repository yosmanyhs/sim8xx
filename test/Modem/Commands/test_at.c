#include "at.h"
#include "test_at_static.h"
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
  At_t at;
  AtObjectInit(&at);

  TEST_ASSERT_EQUAL(&at, at.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(AtSerialize, at.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(AtParse, at.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(AtTimeout, at.atcmd.timeout);
  TEST_ASSERT_EQUAL(2000, at.atcmd.timeoutInMilliSec);
}

void test_AtGetAtCommand(void)
{
  At_t at;
  AtObjectInit(&at);

  AT_Command_t *atcmd = AtGetAtCommand(&at);

  TEST_ASSERT_EQUAL(&at, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(AtSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(AtParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(AtTimeout, at.atcmd.timeout);
  TEST_ASSERT_EQUAL(2000, atcmd->timeoutInMilliSec);
}

void test_AtSerialize(void)
{
  At_t at;
  AtObjectInit(&at);

  char obuf[32] = {0};
  size_t n      = AtSerialize(&at, obuf, sizeof(obuf));

  const char *expected = "AT\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_AtParse_NoEcho(void)
{
  At_t at;
  AtObjectInit(&at);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = AtParse(&at, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, at.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AtParse_WithEcho(void)
{
  At_t at;
  AtObjectInit(&at);

  const char *ibuf = "AT\r\r\nOK\r\n";
  size_t ilen = strlen(ibuf);

  size_t n = AtParse(&at, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, at.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AtParse_Incomplete(void)
{
  At_t at;
  AtObjectInit(&at);

  const char *ibuf = "\r\nOK\r";
  size_t length    = strlen(ibuf);

  size_t n = AtParse(&at, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, at.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_AtParse_InvalidStatus(void)
{
  At_t at;
  AtObjectInit(&at);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = AtParse(&at, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, at.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_AtTimeout(void)
{
  At_t at;
  AtObjectInit(&at);

  AtTimeout(&at);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, at.response.status);
}
