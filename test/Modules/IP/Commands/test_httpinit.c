#include "httpinit.h"
#include "test_httpinit_static.h"
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

void test_HttpinitObjectInit(void)
{
  Httpinit_t httpinit;
  HttpinitObjectInit(&httpinit);

  TEST_ASSERT_EQUAL(&httpinit, httpinit.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(HttpinitSerialize, httpinit.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(HttpinitParse, httpinit.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(HttpinitTimeout, httpinit.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, httpinit.atcmd.timeoutInMilliSec);
}

void test_HttpinitGetAtCommand(void)
{
  Httpinit_t httpinit;
  HttpinitObjectInit(&httpinit);

  AT_Command_t *atcmd = HttpinitGetAtCommand(&httpinit);

  TEST_ASSERT_EQUAL(&httpinit, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(HttpinitSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(HttpinitParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(HttpinitTimeout, httpinit.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_HttpinitSerialize(void)
{
  Httpinit_t httpinit;
  HttpinitObjectInit(&httpinit);

  char obuf[32] = {0};
  size_t n      = HttpinitSerialize(&httpinit, obuf, sizeof(obuf));

  const char *expected = "AT+HTTPINIT\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_HttpinitParse_OK(void)
{
  Httpinit_t httpinit;
  HttpinitObjectInit(&httpinit);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = HttpinitParse(&httpinit, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, httpinit.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_HttpinitParse_ERROR(void)
{
  Httpinit_t httpinit;
  HttpinitObjectInit(&httpinit);

  const char *ibuf = "\r\n+CME ERROR: 24\r\n";
  size_t ilen      = strlen(ibuf);

  size_t n = HttpinitParse(&httpinit, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, httpinit.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_HttpinitParse_Incomplete(void)
{
  Httpinit_t httpinit;
  HttpinitObjectInit(&httpinit);

  const char *ibuf = "\r\nOK";
  size_t length    = strlen(ibuf);

  size_t n = HttpinitParse(&httpinit, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, httpinit.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_HttpinitParse_InvalidStatus(void)
{
  Httpinit_t httpinit;
  HttpinitObjectInit(&httpinit);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = HttpinitParse(&httpinit, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, httpinit.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_HttpinitTimeout(void)
{
  Httpinit_t httpinit;
  HttpinitObjectInit(&httpinit);

  HttpinitTimeout(&httpinit);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, httpinit.response.status);
}
