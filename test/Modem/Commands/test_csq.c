#include "csq.h"
#include "test_csq_static.h"
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

void test_CsqObjectInit(void)
{
  Csq_t csq;
  CsqObjectInit(&csq);

  TEST_ASSERT_EQUAL(&csq, csq.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CsqSerialize, csq.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CsqParse, csq.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(CsqTimeout, csq.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, csq.atcmd.timeoutInMilliSec);
}

void test_CsqGetAtCommand(void)
{
  Csq_t csq;
  CsqObjectInit(&csq);

  AT_Command_t *atcmd = CsqGetAtCommand(&csq);

  TEST_ASSERT_EQUAL(&csq, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CsqSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CsqParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(CsqTimeout, csq.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_CsqSerialize(void)
{
  Csq_t csq;
  CsqObjectInit(&csq);

  char obuf[32] = {0};
  size_t n      = CsqSerialize(&csq, obuf, sizeof(obuf));

  const char *expected = "AT+CSQ\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_CsqParse_OK(void)
{
  Csq_t csq;
  CsqObjectInit(&csq);

  const char *ibuf = "\r\n+CSQ: 24,7\r\n\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CsqParse(&csq, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, csq.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CsqParse_ERROR(void)
{
  Csq_t csq;
  CsqObjectInit(&csq);

  const char *ibuf = "\r\n+CME ERROR: 127\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CsqParse(&csq, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, csq.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CsqParse_Incomplete(void)
{
  Csq_t csq;
  CsqObjectInit(&csq);

  const char *ibuf = "\r\nOK";
  size_t length    = strlen(ibuf);

  size_t n = CsqParse(&csq, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, csq.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CsqParse_InvalidStatus(void)
{
  Csq_t csq;
  CsqObjectInit(&csq);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CsqParse(&csq, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, csq.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CsqTimeout(void)
{
  Csq_t csq;
  CsqObjectInit(&csq);

  CsqTimeout(&csq);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, csq.response.status);
}
