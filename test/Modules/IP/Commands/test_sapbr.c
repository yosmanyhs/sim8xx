#include "sapbr.h"
#include "test_sapbr_static.h"
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

void test_SapbrObjectInit(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);

  TEST_ASSERT_EQUAL(&sapbr, sapbr.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(SapbrSerialize, sapbr.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(SapbrParse, sapbr.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(SapbrTimeout, sapbr.atcmd.timeout);
  TEST_ASSERT_EQUAL(85000, sapbr.atcmd.timeoutInMilliSec);
}

void test_SapbrGetAtCommand(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);

  AT_Command_t *atcmd = SapbrGetAtCommand(&sapbr);

  TEST_ASSERT_EQUAL(&sapbr, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(SapbrSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(SapbrParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(SapbrTimeout, sapbr.atcmd.timeout);
  TEST_ASSERT_EQUAL(85000, atcmd->timeoutInMilliSec);
}

void test_SapbrSetupRequestSetConnTypeGprs(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestSetConnTypeGprs(&sapbr, 1);

  TEST_ASSERT_EQUAL(CMD_TYPE_SET, sapbr.request.cmd);
  TEST_ASSERT_EQUAL(1, sapbr.request.id);
  TEST_ASSERT_EQUAL(PARAM_TAG_CONNTYPE, sapbr.request.tag);
  TEST_ASSERT_EQUAL_STRING("GPRS", sapbr.request.value);
}

void test_SapbrSetupRequestSetApn(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestSetApn(&sapbr, 1, "example.internet.net");

  TEST_ASSERT_EQUAL(CMD_TYPE_SET, sapbr.request.cmd);
  TEST_ASSERT_EQUAL(1, sapbr.request.id);
  TEST_ASSERT_EQUAL(PARAM_TAG_APN, sapbr.request.tag);
  TEST_ASSERT_EQUAL_STRING("example.internet.net", sapbr.request.value);
}

void test_SapbrSetupRequestOpenContext(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestOpenContext(&sapbr, 1);

  TEST_ASSERT_EQUAL(CMD_TYPE_OPEN, sapbr.request.cmd);
  TEST_ASSERT_EQUAL(1, sapbr.request.id);
}

void test_SapbrSetupRequestCloseContext(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestCloseContext(&sapbr, 1);

  TEST_ASSERT_EQUAL(CMD_TYPE_CLOSE, sapbr.request.cmd);
  TEST_ASSERT_EQUAL(1, sapbr.request.id);
}

void test_SapbrSerialize_CLOSE(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);

  sapbr.request.cmd = CMD_TYPE_CLOSE;
  sapbr.request.id = 1;

  char obuf[32] = {0};
  size_t n      = SapbrSerialize(&sapbr, obuf, sizeof(obuf));

  const char *expected = "AT+SAPBR=0,1\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_SapbrSerialize_OPEN(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  sapbr.request.cmd = CMD_TYPE_OPEN;
  sapbr.request.id = 1;

  char obuf[32] = {0};
  size_t n      = SapbrSerialize(&sapbr, obuf, sizeof(obuf));

  const char *expected = "AT+SAPBR=1,1\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_SapbrSerialize_QUERY(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);

  sapbr.request.cmd = CMD_TYPE_QUERY;
  sapbr.request.id = 1;

  char obuf[32] = {0};
  size_t n      = SapbrSerialize(&sapbr, obuf, sizeof(obuf));

  const char *expected = "AT+SAPBR=2,1\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_SapbrSerialize_SET(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  sapbr.request.cmd = CMD_TYPE_SET;
  sapbr.request.id = 1;
  sapbr.request.tag = PARAM_TAG_APN;
  sapbr.request.value = "example.internet.net";

  char obuf[50] = {0};
  size_t n      = SapbrSerialize(&sapbr, obuf, sizeof(obuf));

  const char *expected = "AT+SAPBR=3,1,\"APN\",\"example.internet.net\"\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_SapbrSerialize_GET(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  sapbr.request.cmd = CMD_TYPE_GET;
  sapbr.request.id = 1;
  sapbr.request.tag = PARAM_TAG_APN;

  char obuf[50] = {0};
  size_t n      = SapbrSerialize(&sapbr, obuf, sizeof(obuf));

  const char *expected = "AT+SAPBR=4,1,\"APN\"\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_SapbrSetupRequestSetConnTypeGprs_SapbrSerialize(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestSetConnTypeGprs(&sapbr, 1);

  char obuf[50] = {0};
  size_t n      = SapbrSerialize(&sapbr, obuf, sizeof(obuf));

  const char *expected = "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_SapbrSetupRequestSetApn_SapbrSerialize(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestSetApn(&sapbr, 1, "example.internet.net");

  char obuf[50] = {0};
  size_t n      = SapbrSerialize(&sapbr, obuf, sizeof(obuf));

  const char *expected = "AT+SAPBR=3,1,\"APN\",\"example.internet.net\"\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_SapbrSetupRequestOpenContext_SapbrSerialize(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestOpenContext(&sapbr, 1);

  char obuf[50] = {0};
  size_t n      = SapbrSerialize(&sapbr, obuf, sizeof(obuf));

  const char *expected = "AT+SAPBR=1,1\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_SapbrSetupRequestCloseContext_SapbrSerialize(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestCloseContext(&sapbr, 1);

  char obuf[50] = {0};
  size_t n      = SapbrSerialize(&sapbr, obuf, sizeof(obuf));

  const char *expected = "AT+SAPBR=0,1\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_SapbrParse_CLOSE_OK(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  sapbr.request.cmd = CMD_TYPE_CLOSE;
  sapbr.request.id = 1;

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = SapbrParse(&sapbr, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, sapbr.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_SapbrParse_OPEN_OK(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  sapbr.request.cmd = CMD_TYPE_OPEN;
  sapbr.request.id = 1;
  
  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = SapbrParse(&sapbr, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, sapbr.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_SapbrParse_QUERY_OK(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  sapbr.request.cmd = CMD_TYPE_QUERY;
  sapbr.request.id = 1;
  
  const char *ibuf = "\r\n+SAPBR:1,2,\"10.89.193.1\"\r\n\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = SapbrParse(&sapbr, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, sapbr.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(1, sapbr.response.payload.query.id);
  TEST_ASSERT_EQUAL(2, sapbr.response.payload.query.status);
  TEST_ASSERT_EQUAL_STRING("10.89.193.1", sapbr.response.payload.query.ipaddr);
}

void test_SapbrParse_SET_OK(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  sapbr.request.cmd = CMD_TYPE_SET;
  sapbr.request.id = 1;
  
  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = SapbrParse(&sapbr, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, sapbr.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_SapbrParse_GET_OK(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  sapbr.request.cmd = CMD_TYPE_GET;
  sapbr.request.tag = PARAM_TAG_APN;
  
  const char *ibuf = "\r\n+SAPBR:\"APN\",\"example.internet.net\"\r\n\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = SapbrParse(&sapbr, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, sapbr.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL_STRING("APN", sapbr.response.payload.get.tag);
  TEST_ASSERT_EQUAL_STRING("example.internet.net", sapbr.response.payload.get.value);
}

void test_SapbrParse_ERROR(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);

  const char *ibuf = "\r\nERROR\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = SapbrParse(&sapbr, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, sapbr.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_SapbrParse_Incomplete(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);

  const char *ibuf = "\r\nOK";
  size_t length    = strlen(ibuf);

  size_t n = SapbrParse(&sapbr, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, sapbr.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_SapbrParse_InvalidStatus(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = SapbrParse(&sapbr, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, sapbr.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_SapbrTimeout(void)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);

  SapbrTimeout(&sapbr);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, sapbr.response.status);
}

void test_SapbrIsURC(void)
{
  const char *ibuf = "\r\n+SAPBR 4: DEACT\r\n";
  size_t length    = strlen(ibuf);

  TEST_ASSERT(SapbrIsURC(ibuf, length));
}

void test_SapbrParseURC(void)
{
  const char *ibuf = "\r\n+SAPBR 4: DEACT\r\n";
  size_t length    = strlen(ibuf);
  SapbrURC_t urc;

  size_t n = SapbrParseURC(&urc, ibuf, length);

  TEST_ASSERT_EQUAL(length, n);
  TEST_ASSERT_EQUAL(SAPBR_URC_DEACT, urc.type);
  TEST_ASSERT_EQUAL(4, urc.payload.deact.id);
}

void test_SapbrParseURC_MissingId(void)
{
  const char *ibuf = "\r\n+SAPBR: DEACT\r\n";
  size_t length    = strlen(ibuf);
  SapbrURC_t urc;

  size_t n = SapbrParseURC(&urc, ibuf, length);

  TEST_ASSERT_EQUAL(0, n);
  TEST_ASSERT_EQUAL(SAPBR_URC_NO_URC, urc.type);
  TEST_ASSERT_EQUAL(0, urc.payload.deact.id);
}