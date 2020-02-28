#include "httpaction.h"
#include "test_httpaction_static.h"
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

void test_HttpactionObjectInit(void)
{
  Httpaction_t httpaction;
  HttpactionObjectInit(&httpaction);

  TEST_ASSERT_EQUAL(&httpaction, httpaction.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(HttpactionSerialize, httpaction.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(HttpactionParse, httpaction.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(HttpactionTimeout, httpaction.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, httpaction.atcmd.timeoutInMilliSec);
}

void test_HttpactionGetAtCommand(void)
{
  Httpaction_t httpaction;
  HttpactionObjectInit(&httpaction);

  AT_Command_t *atcmd = HttpactionGetAtCommand(&httpaction);

  TEST_ASSERT_EQUAL(&httpaction, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(HttpactionSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(HttpactionParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(HttpactionTimeout, httpaction.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_HttpactionSerialize_GET(void)
{
  Httpaction_t httpaction;
  HttpactionObjectInit(&httpaction);
  HttpactionSetupRequest(&httpaction, HTTPACTION_GET);

  char obuf[32] = {0};
  size_t n      = HttpactionSerialize(&httpaction, obuf, sizeof(obuf));

  const char *expected = "AT+HTTPACTION=0\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_HttpactionSerialize_POST(void)
{
  Httpaction_t httpaction;
  HttpactionObjectInit(&httpaction);
  HttpactionSetupRequest(&httpaction, HTTPACTION_POST);

  char obuf[32] = {0};
  size_t n      = HttpactionSerialize(&httpaction, obuf, sizeof(obuf));

  const char *expected = "AT+HTTPACTION=1\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_HttpactionSerialize_HEAD(void)
{
  Httpaction_t httpaction;
  HttpactionObjectInit(&httpaction);
  HttpactionSetupRequest(&httpaction, HTTPACTION_HEAD);

  char obuf[32] = {0};
  size_t n      = HttpactionSerialize(&httpaction, obuf, sizeof(obuf));

  const char *expected = "AT+HTTPACTION=2\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_HttpactionParse_OK(void)
{
  Httpaction_t httpaction;
  HttpactionObjectInit(&httpaction);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = HttpactionParse(&httpaction, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, httpaction.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_HttpactionParse_ERROR(void)
{
  Httpaction_t httpaction;
  HttpactionObjectInit(&httpaction);

  const char *ibuf = "\r\n+CME ERROR\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = HttpactionParse(&httpaction, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, httpaction.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_HttpactionParse_Incomplete(void)
{
  Httpaction_t httpaction;
  HttpactionObjectInit(&httpaction);

  const char *ibuf = "\r\nOK";
  size_t length    = strlen(ibuf);

  size_t n = HttpactionParse(&httpaction, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, httpaction.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_HttpactionParse_InvalidStatus(void)
{
  Httpaction_t httpaction;
  HttpactionObjectInit(&httpaction);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = HttpactionParse(&httpaction, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, httpaction.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_HttpactionTimeout(void)
{
  Httpaction_t httpaction;
  HttpactionObjectInit(&httpaction);

  HttpactionTimeout(&httpaction);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, httpaction.response.status);
}

void test_HttpactionIsURC(void)
{
  const char *ibuf = "\r\n+HTTPACTION: 0,200,1000\r\n";
  size_t length    = strlen(ibuf);

  TEST_ASSERT(HttpactionIsURC(ibuf, length));
}

void test_HttpactionParseURC(void)
{
  const char *ibuf = "\r\n+HTTPACTION: 0,200,1000\r\n";
  size_t length    = strlen(ibuf);

  HttpactionURC_t urc;
  size_t n = HttpactionParseURC(&urc, ibuf, length);

  TEST_ASSERT_EQUAL(length, n);
  TEST_ASSERT_EQUAL_INT32(0, urc.method);
  TEST_ASSERT_EQUAL_INT32(200, urc.httpStatus);
  TEST_ASSERT_EQUAL_INT32(1000, urc.dataLength);
}