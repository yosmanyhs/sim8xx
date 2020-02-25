#include "cipclose.h"
#include "test_cipclose_static.h"
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

void test_CipcloseObjectInit(void)
{
  Cipclose_t cipclose;
  CipcloseObjectInit(&cipclose);

  TEST_ASSERT_EQUAL(&cipclose, cipclose.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CipcloseSerialize, cipclose.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CipcloseParse, cipclose.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(CipcloseTimeout, cipclose.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, cipclose.atcmd.timeoutInMilliSec);
}

void test_CipcloseGetAtCommand(void)
{
  Cipclose_t cipclose;
  CipcloseObjectInit(&cipclose);

  AT_Command_t *atcmd = CipcloseGetAtCommand(&cipclose);

  TEST_ASSERT_EQUAL(&cipclose, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CipcloseSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CipcloseParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(CipcloseTimeout, cipclose.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_CipcloseSerialize(void)
{
  Cipclose_t cipclose;
  CipcloseObjectInit(&cipclose);

  char obuf[32] = {0};
  size_t n      = CipcloseSerialize(&cipclose, obuf, sizeof(obuf));

  const char *expected = "AT+CIPCLOSE\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_CipcloseParse_OK(void)
{
  Cipclose_t cipclose;
  CipcloseObjectInit(&cipclose);

  const char *ibuf = "\r\nCLOSE OK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CipcloseParse(&cipclose, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, cipclose.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CipcloseParse_ERROR(void)
{
  Cipclose_t cipclose;
  CipcloseObjectInit(&cipclose);

  const char *ibuf = "\r\nERROR\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CipcloseParse(&cipclose, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, cipclose.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CipcloseParse_Incomplete(void)
{
  Cipclose_t cipclose;
  CipcloseObjectInit(&cipclose);

  const char *ibuf = "\r\nCLOSE OK";
  size_t length    = strlen(ibuf);

  size_t n = CipcloseParse(&cipclose, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cipclose.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CipcloseParse_InvalidStatus(void)
{
  Cipclose_t cipclose;
  CipcloseObjectInit(&cipclose);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CipcloseParse(&cipclose, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cipclose.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CipcloseTimeout(void)
{
  Cipclose_t cipclose;
  CipcloseObjectInit(&cipclose);

  CipcloseTimeout(&cipclose);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, cipclose.response.status);
}
