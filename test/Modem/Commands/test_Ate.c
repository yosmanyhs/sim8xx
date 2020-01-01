#include "Ate.h"
#include "test_Ate_static.h"
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

void test_AteObjectInit(void)
{
  Ate_t ate;
  AteObjectInit(&ate);

  TEST_ASSERT_EQUAL(&ate, ate.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(AteSerialize, ate.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(AteParse, ate.atcmd.parse);
  TEST_ASSERT_EQUAL(5, ate.atcmd.timeout);
}

void test_AteGetAtCommand(void)
{
  Ate_t ate;
  AteObjectInit(&ate);

  AT_Command_t *atcmd = AteGetAtCommand(&ate);

  TEST_ASSERT_EQUAL(&ate, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR( AteSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR( AteParse, atcmd->parse);
  TEST_ASSERT_EQUAL(5, atcmd->timeout);
}

void test_AteSetupRequest_Mode0(void)
{
  Ate_t ate;
  AteObjectInit(&ate);
  AteSetupRequest(&ate, 0);

  TEST_ASSERT_EQUAL(0, ate.request.mode);
}

void test_AteSetupRequest_Mode1(void)
{
  Ate_t ate;
  AteObjectInit(&ate);
  AteSetupRequest(&ate, 1);

  TEST_ASSERT_EQUAL(1, ate.request.mode);
}

void test_AteSerialize_Mode0(void)
{
  Ate_t ate;
  AteObjectInit(&ate);
  AteSetupRequest(&ate, 0);

  char obuf[32] = {0};
  size_t n      =  AteSerialize(&ate, obuf, sizeof(obuf));

  const char *expected = "ATE0";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_AteSerialize_Mode1(void)
{
  Ate_t ate;
  AteObjectInit(&ate);
  AteSetupRequest(&ate, 1);

  char obuf[32] = {0};
  size_t n      =  AteSerialize(&ate, obuf, sizeof(obuf));

  const char *expected = "ATE1";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_AteParse_Mode0_NoEcho(void)
{
  Ate_t ate;
  AteObjectInit(&ate);
  AteSetupRequest(&ate, 0);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen = strlen(ibuf);

  size_t n = AteParse(&ate, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, ate.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AteParse_Mode1_NoEcho(void)
{
  Ate_t ate;
  AteObjectInit(&ate);
  AteSetupRequest(&ate, 1);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen = strlen(ibuf);

  size_t n = AteParse(&ate, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, ate.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AteParse_Mode0_WithEcho(void)
{
  Ate_t ate;
  AteObjectInit(&ate);
  AteSetupRequest(&ate, 0);

  const char *ibuf = "ATE0\r\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n =  AteParse(&ate, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, ate.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AteParse_Mode1_WithEcho(void)
{
  Ate_t ate;
  AteObjectInit(&ate);
  AteSetupRequest(&ate, 1);

  const char *ibuf = "ATE1\r\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n =  AteParse(&ate, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, ate.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AteParse_Incomplete(void)
{
  Ate_t ate;
  AteObjectInit(&ate);

  const char *ibuf = "\r\nOK\r";
  size_t length    = strlen(ibuf);

  size_t n = AteParse(&ate, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, ate.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_AteParse_InvalidStatus(void)
{
  Ate_t ate;
  AteObjectInit(&ate);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n =  AteParse(&ate, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, ate.response.status);
  TEST_ASSERT_EQUAL(0, n);
}
