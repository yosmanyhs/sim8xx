#include "cipsend.h"
#include "test_cipsend_static.h"
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

void test_CipsendObjectInit(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);

  TEST_ASSERT_EQUAL(&cipsend, cipsend.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CipsendSerialize, cipsend.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CipsendParse, cipsend.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(CipsendTimeout, cipsend.atcmd.timeout);
  TEST_ASSERT_EQUAL(645000, cipsend.atcmd.timeoutInMilliSec);
  TEST_ASSERT_EQUAL(CIPSEND_COMMAND, cipsend.mode);
}

void test_CipsendGetAtCommand(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);

  AT_Command_t *atcmd = CipsendGetAtCommand(&cipsend);

  TEST_ASSERT_EQUAL(&cipsend, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CipsendSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CipsendParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(CipsendTimeout, cipsend.atcmd.timeout);
  TEST_ASSERT_EQUAL(645000, atcmd->timeoutInMilliSec);
}

void test_CipsendSetCommandMode(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);
  CipsendSetCommandMode(&cipsend);

  TEST_ASSERT_EQUAL(CIPSEND_COMMAND, cipsend.mode);
}

void test_CipsendDataCommandMode(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);
  CipsendSetDataMode(&cipsend);

  TEST_ASSERT_EQUAL(CIPSEND_DATA, cipsend.mode);
}

void test_CipsendSerialize_CommandMode(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);
  CipsendSetCommandMode(&cipsend);
  const char *msg = "test message";
  CipsendSetupRequest(&cipsend, msg, strlen(msg));

  char obuf[32] = {0};
  size_t n      = CipsendSerialize(&cipsend, obuf, sizeof(obuf));

  const char *expected = "AT+CIPSEND\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_CipsendSerialize_DataMode(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);
  CipsendSetDataMode(&cipsend);
  const char *msg = "test message";
  CipsendSetupRequest(&cipsend, msg, strlen(msg));

  char obuf[32] = {0};
  size_t n      = CipsendSerialize(&cipsend, obuf, sizeof(obuf));

  const char *expected = "test message\x1A";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_CipsendParse_WAIT_USER_DATA(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);
  CipsendSetCommandMode(&cipsend);

  const char *ibuf = "\r\n> ";
  size_t length    = strlen(ibuf);

  size_t n = CipsendParse(&cipsend, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_WAIT_FOR_USER_DATA, cipsend.response.status);
  TEST_ASSERT_EQUAL(length, n);
}

void test_CipsendParse_SEND_OK(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);
  CipsendSetDataMode(&cipsend);

  const char *ibuf = "\r\nSEND OK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CipsendParse(&cipsend, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_SEND_OK, cipsend.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CipsendParse_SEND_FAIL(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);
  CipsendSetDataMode(&cipsend);

  const char *ibuf = "\r\nSEND FAIL\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CipsendParse(&cipsend, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_SEND_FAIL, cipsend.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CipsendParse_Incomplete(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);

  const char *ibuf = "\r\nOK";
  size_t length    = strlen(ibuf);

  size_t n = CipsendParse(&cipsend, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cipsend.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CipsendParse_InvalidStatus(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CipsendParse(&cipsend, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cipsend.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CipsendTimeout(void)
{
  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);

  CipsendTimeout(&cipsend);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, cipsend.response.status);
}

void test_CipsendIsURC_SEND_OK(void)
{
  const char *ibuf = "\r\nSEND OK\r\n";
  size_t length    = strlen(ibuf);

  TEST_ASSERT(CipsendIsURC(ibuf, length));
}

void test_CipsendIsURC_SEND_FAIL(void)
{
  const char *ibuf = "\r\nSEND FAIL\r\n";
  size_t length    = strlen(ibuf);

  TEST_ASSERT(CipsendIsURC(ibuf, length));
}

void test_CipsendIsURC_NoURC(void)
{
  const char *ibuf = "\r\nsomething else\r\n";
  size_t length    = strlen(ibuf);

  TEST_ASSERT_FALSE(CipsendIsURC(ibuf, length));
}

void test_CipsendParseURC_SEND_OK(void)
{
  CipsendURC_t urc = {0};

  const char *ibuf = "\r\nSEND OK\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CipsendParseURC(&urc, ibuf, length);

  TEST_ASSERT_EQUAL(CIPSEND_SEND_OK, urc.type);
  TEST_ASSERT_EQUAL(length, n);
}

void test_CipsendParseURC_SEND_FAIL(void)
{
  CipsendURC_t urc = {0};

  const char *ibuf = "\r\nSEND FAIL\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CipsendParseURC(&urc, ibuf, length);

  TEST_ASSERT_EQUAL(CIPSEND_SEND_FAIL, urc.type);
  TEST_ASSERT_EQUAL(length, n);
}

void test_CipsendParseURC_NO_URC(void)
{
  CipsendURC_t urc = {0};

  const char *ibuf = "\r\nsomething else\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CipsendParseURC(&urc, ibuf, length);

  TEST_ASSERT_EQUAL(CIPSEND_NO_URC, urc.type);
  TEST_ASSERT_EQUAL(0, n);
}