#include "cipshut.h"
#include "test_cipshut_static.h"
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

void test_CipshutObjectInit(void)
{
  Cipshut_t cipshut;
  CipshutObjectInit(&cipshut);

  TEST_ASSERT_EQUAL(&cipshut, cipshut.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CipshutSerialize, cipshut.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CipshutParse, cipshut.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(CipshutTimeout, cipshut.atcmd.timeout);
  TEST_ASSERT_EQUAL(65000, cipshut.atcmd.timeoutInMilliSec);
}

void test_CipshutGetAtCommand(void)
{
  Cipshut_t cipshut;
  CipshutObjectInit(&cipshut);

  AT_Command_t *atcmd = CipshutGetAtCommand(&cipshut);

  TEST_ASSERT_EQUAL(&cipshut, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CipshutSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CipshutParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(CipshutTimeout, cipshut.atcmd.timeout);
  TEST_ASSERT_EQUAL(65000, atcmd->timeoutInMilliSec);
}

void test_CipshutSerialize(void)
{
  Cipshut_t cipshut;
  CipshutObjectInit(&cipshut);

  char obuf[32] = {0};
  size_t n      = CipshutSerialize(&cipshut, obuf, sizeof(obuf));

  const char *expected = "AT+CIPSHUT\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_CipshutParse_OK(void)
{
  Cipshut_t cipshut;
  CipshutObjectInit(&cipshut);

  const char *ibuf = "\r\nSHUT OK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CipshutParse(&cipshut, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, cipshut.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CipshutParse_ERROR(void)
{
  Cipshut_t cipshut;
  CipshutObjectInit(&cipshut);

  const char *ibuf = "\r\nERROR\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CipshutParse(&cipshut, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, cipshut.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_CipshutParse_Incomplete(void)
{
  Cipshut_t cipshut;
  CipshutObjectInit(&cipshut);

  const char *ibuf = "\r\nOK";
  size_t length    = strlen(ibuf);

  size_t n = CipshutParse(&cipshut, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cipshut.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CipshutParse_InvalidStatus(void)
{
  Cipshut_t cipshut;
  CipshutObjectInit(&cipshut);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CipshutParse(&cipshut, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cipshut.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CipshutTimeout(void)
{
  Cipshut_t cipshut;
  CipshutObjectInit(&cipshut);

  CipshutTimeout(&cipshut);
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, cipshut.response.status);
}
