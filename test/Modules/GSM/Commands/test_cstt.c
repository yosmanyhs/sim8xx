#include "cstt.h"
#include "test_cstt_static.h"
#include "unity.h"

#include <string.h>

TEST_FILE("AtCommand.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_CsttObjectInit(void)
{
  Cstt_t cstt;
  CsttObjectInit(&cstt);

  TEST_ASSERT_EQUAL(&cstt, cstt.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CsttSerialize, cstt.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CsttParse, cstt.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(CsttTimeout, cstt.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, cstt.atcmd.timeoutInMilliSec);
}

void test_CsttSetupRequest_ApnOnly(void)
{
  Cstt_t cstt;
  CsttObjectInit(&cstt);
  CsttSetupRequest(&cstt, "testapn", NULL, NULL);

  TEST_ASSERT_EQUAL_STRING("testapn", cstt.request.apn);
  TEST_ASSERT_EQUAL_PTR(NULL, cstt.request.user);
  TEST_ASSERT_EQUAL_PTR(NULL, cstt.request.passwd);
}

void test_CsttSetupRequest_ApnUserPasswd(void)
{
  Cstt_t cstt;
  CsttObjectInit(&cstt);
  CsttSetupRequest(&cstt, "testapn", "testuser", "testpasswd");

  TEST_ASSERT_EQUAL_STRING("testapn", cstt.request.apn);
  TEST_ASSERT_EQUAL_PTR("testuser", cstt.request.user);
  TEST_ASSERT_EQUAL_PTR("testpasswd", cstt.request.passwd);
}

void test_CsttGetAtCommand(void)
{
  Cstt_t cstt;
  CsttObjectInit(&cstt);

  AT_Command_t *atcmd = CsttGetAtCommand(&cstt);

  TEST_ASSERT_EQUAL(&cstt, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CsttSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CsttParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(CsttTimeout, cstt.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_CsttSerialize_ApnOnly(void)
{
  Cstt_t cstt;
  CsttObjectInit(&cstt);
  CsttSetupRequest(&cstt, "internet.vodafone.net", NULL, NULL);

  char obuf[50] = {0};
  size_t n      = CsttSerialize(&cstt, obuf, sizeof(obuf));

  const char *expected = "AT+CSTT=\"internet.vodafone.net\"\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_CsttSerialize_ApnUserPasswd(void)
{
  Cstt_t cstt;
  CsttObjectInit(&cstt);
  CsttSetupRequest(&cstt, "internet.vodafone.net", "voda", "fone");

  char obuf[50] = {0};
  size_t n      = CsttSerialize(&cstt, obuf, sizeof(obuf));

  const char *expected = "AT+CSTT=\"internet.vodafone.net\",\"voda\",\"fone\"\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_CsttParse_Incomplete(void)
{
  Cstt_t cstt;
  CsttObjectInit(&cstt);

  const char *ibuf = "\r\nOK\r";
  size_t length    = strlen(ibuf);

  size_t n = CsttParse(&cstt, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cstt.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CsttParse_OK(void)
{
  Cstt_t cstt;
  CsttObjectInit(&cstt);

  const char *ibuf = "\r\nOK\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CsttParse(&cstt, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_OK, cstt.response.status);
  TEST_ASSERT_EQUAL(length, n);
}

void test_CsttParse_ERROR(void)
{
  Cstt_t cstt;
  CsttObjectInit(&cstt);

  const char *ibuf = "\r\nERROR\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CsttParse(&cstt, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, cstt.response.status);
  TEST_ASSERT_EQUAL(length, n);
}

void test_CsttParse_InvalidStatus(void)
{
  Cstt_t cstt;
  CsttObjectInit(&cstt);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CsttParse(&cstt, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cstt.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CsttTimeout(void)
{
  Cstt_t cstt;
  CsttObjectInit(&cstt);

  CsttTimeout(&cstt);

  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, cstt.response.status);
}
