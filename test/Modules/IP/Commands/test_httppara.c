#include "httppara.h"
#include "test_httppara_static.h"
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

void test_HttpparaObjectInit(void)
{
  Httppara_t httppara;
  HttpparaObjectInit(&httppara);

  TEST_ASSERT_EQUAL(&httppara, httppara.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(HttpparaSerialize, httppara.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(HttpparaParse, httppara.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(HttpparaTimeout, httppara.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, httppara.atcmd.timeoutInMilliSec);
}

void test_HttpparaGetAtCommand(void)
{
  Httppara_t httppara;
  HttpparaObjectInit(&httppara);

  AT_Command_t *atcmd = HttpparaGetAtCommand(&httppara);

  TEST_ASSERT_EQUAL(&httppara, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(HttpparaSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(HttpparaParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(HttpparaTimeout, httppara.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_HttpparaSerialize_CID(void)
{
  Httppara_t httppara;
  HttpparaObjectInit(&httppara);
  HttpparaSetupRequestSetCid(&httppara, "1");

  char obuf[32] = {0};
  size_t n      = HttpparaSerialize(&httppara, obuf, sizeof(obuf));

  const char *expected = "AT+HTTPPARA=\"CID\",1\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_HttpparaSerialize_URL(void)
{
  Httppara_t httppara;
  HttpparaObjectInit(&httppara);
  HttpparaSetupRequestSetUrl(&httppara, "www.melcontrol.com");

  char obuf[50] = {0};
  size_t n      = HttpparaSerialize(&httppara, obuf, sizeof(obuf));

  const char *expected = "AT+HTTPPARA=\"URL\",\"www.melcontrol.com\"\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_HttpparaParse_OK(void)
{
  Httppara_t httppara;
  HttpparaObjectInit(&httppara);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = HttpparaParse(&httppara, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, httppara.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_HttpparaParse_ERROR(void)
{
  Httppara_t httppara;
  HttpparaObjectInit(&httppara);

  const char *ibuf = "\r\n+CME ERROR\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = HttpparaParse(&httppara, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, httppara.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_HttpparaParse_Incomplete(void)
{
  Httppara_t httppara;
  HttpparaObjectInit(&httppara);

  const char *ibuf = "\r\nOK";
  size_t length    = strlen(ibuf);

  size_t n = HttpparaParse(&httppara, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, httppara.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_HttpparaParse_InvalidStatus(void)
{
  Httppara_t httppara;
  HttpparaObjectInit(&httppara);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = HttpparaParse(&httppara, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, httppara.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_HttpparaTimeout(void)
{
  Httppara_t httppara;
  HttpparaObjectInit(&httppara);

  HttpparaTimeout(&httppara);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, httppara.response.status);
}
