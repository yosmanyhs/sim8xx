#include "cipstart.h"
#include "test_cipstart_static.h"
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

void test_CipstartObjectInit(void)
{
  Cipstart_t cipstart;
  CipstartObjectInit(&cipstart);

  TEST_ASSERT_EQUAL(&cipstart, cipstart.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CipstartSerialize, cipstart.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CipstartParse, cipstart.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(CipstartTimeout, cipstart.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, cipstart.atcmd.timeoutInMilliSec);
}

void test_CipstartGetAtCommand(void)
{
  Cipstart_t cipstart;
  CipstartObjectInit(&cipstart);

  AT_Command_t *atcmd = CipstartGetAtCommand(&cipstart);

  TEST_ASSERT_EQUAL(&cipstart, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CipstartSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CipstartParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(CipstartTimeout, cipstart.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_CipstartSerialize(void)
{
  Cipstart_t cipstart;
  CipstartObjectInit(&cipstart);
  CipstartSetupRequest(&cipstart, "TCP", "example.host.com", 5559);

  char obuf[50] = {0};
  size_t n      = CipstartSerialize(&cipstart, obuf, sizeof(obuf));

  const char *expected = "AT+CIPSTART=\"TCP\",\"example.host.com\",5559\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_CipstartParse_OK(void)
{
  Cipstart_t cipstart;
  CipstartObjectInit(&cipstart);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CipstartParse(&cipstart, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, cipstart.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CipstartParse_ERROR(void)
{
  Cipstart_t cipstart;
  CipstartObjectInit(&cipstart);

  const char *ibuf = "\r\nERROR\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CipstartParse(&cipstart, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, cipstart.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CipstartParse_CME_ERROR(void)
{
  Cipstart_t cipstart;
  CipstartObjectInit(&cipstart);

  const char *ibuf = "\r\n+CME ERROR: 29\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CipstartParse(&cipstart, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, cipstart.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CipstartParse_Incomplete(void)
{
  Cipstart_t cipstart;
  CipstartObjectInit(&cipstart);

  const char *ibuf = "\r\nOK";
  size_t length    = strlen(ibuf);

  size_t n = CipstartParse(&cipstart, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cipstart.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CipstartParse_InvalidStatus(void)
{
  Cipstart_t cipstart;
  CipstartObjectInit(&cipstart);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CipstartParse(&cipstart, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cipstart.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CipstartTimeout(void)
{
  Cipstart_t cipstart;
  CipstartObjectInit(&cipstart);

  CipstartTimeout(&cipstart);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, cipstart.response.status);
}

void test_CipstartIsURC_AlreadyConnect(void)
{
  const char *ibuf = "\r\nALREADY CONNECT\r\n";
  size_t length    = strlen(ibuf);

  TEST_ASSERT(CipstartIsURC(ibuf, length));
}


void test_CipstartIsURC_ConnectOK(void)
{
  const char *ibuf = "\r\nCONNECT OK\r\n";
  size_t length    = strlen(ibuf);

  TEST_ASSERT(CipstartIsURC(ibuf, length));
}

void test_CipstartIsURC_ConnectFAIL(void)
{
  const char *ibuf = "\r\nSTATE: 139\r\n\r\nCONNECT FAIL\r\n";
  size_t length    = strlen(ibuf);

  TEST_ASSERT(CipstartIsURC(ibuf, length));
}

void test_CipstartParseURC_AlreadyConnect(void)
{
  CipstartURC_t urc = {0};

  const char *ibuf = "\r\nALREADY CONNECT\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CipstartParseURC(&urc, ibuf, length);

  TEST_ASSERT_EQUAL(CIPSTART_ALREADY_CONNECT, urc.type);
  TEST_ASSERT_EQUAL(length, n);
}

void test_CipstartParseURC_ConnectOK(void)
{
  CipstartURC_t urc = {0};

  const char *ibuf = "\r\nCONNECT OK\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CipstartParseURC(&urc, ibuf, length);

  TEST_ASSERT_EQUAL(CIPSTART_CONNECT_OK, urc.type);
  TEST_ASSERT_EQUAL(length, n);
}

void test_CipstartParseURC_ConnectFAIL(void)
{
  CipstartURC_t urc = {0};

  const char *ibuf = "\r\nSTATE: 139\r\n\r\nCONNECT FAIL\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CipstartParseURC(&urc, ibuf, length);

  TEST_ASSERT_EQUAL(CIPSTART_CONNECT_FAIL, urc.type);
  TEST_ASSERT_EQUAL(length, n);
  TEST_ASSERT_EQUAL(139, urc.payload.fail.state);
}