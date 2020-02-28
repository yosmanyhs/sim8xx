#include "httpterm.h"
#include "test_httpterm_static.h"
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

void test_HttptermObjectInit(void)
{
  Httpterm_t httpterm;
  HttptermObjectInit(&httpterm);

  TEST_ASSERT_EQUAL(&httpterm, httpterm.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(HttptermSerialize, httpterm.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(HttptermParse, httpterm.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(HttptermTimeout, httpterm.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, httpterm.atcmd.timeoutInMilliSec);
}

void test_HttptermGetAtCommand(void)
{
  Httpterm_t httpterm;
  HttptermObjectInit(&httpterm);

  AT_Command_t *atcmd = HttptermGetAtCommand(&httpterm);

  TEST_ASSERT_EQUAL(&httpterm, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(HttptermSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(HttptermParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(HttptermTimeout, httpterm.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_HttptermSerialize(void)
{
  Httpterm_t httpterm;
  HttptermObjectInit(&httpterm);

  char obuf[32] = {0};
  size_t n      = HttptermSerialize(&httpterm, obuf, sizeof(obuf));

  const char *expected = "AT+HTTPTERM\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_HttptermParse_OK(void)
{
  Httpterm_t httpterm;
  HttptermObjectInit(&httpterm);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = HttptermParse(&httpterm, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, httpterm.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_HttptermParse_ERROR(void)
{
  Httpterm_t httpterm;
  HttptermObjectInit(&httpterm);

  const char *ibuf = "\r\n+CME ERROR: 24\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = HttptermParse(&httpterm, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, httpterm.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_HttptermParse_Incomplete(void)
{
  Httpterm_t httpterm;
  HttptermObjectInit(&httpterm);

  const char *ibuf = "\r\nOK";
  size_t length    = strlen(ibuf);

  size_t n = HttptermParse(&httpterm, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, httpterm.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_HttptermParse_InvalidStatus(void)
{
  Httpterm_t httpterm;
  HttptermObjectInit(&httpterm);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = HttptermParse(&httpterm, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, httpterm.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_HttptermTimeout(void)
{
  Httpterm_t httpterm;
  HttptermObjectInit(&httpterm);

  HttptermTimeout(&httpterm);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, httpterm.response.status);
}
