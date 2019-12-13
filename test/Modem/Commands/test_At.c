#include "At.h"
#include "test_At_static.h"
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

void test_AtObjectInit(void)
{
  At_t at;
  AtObjectInit(&at);

  TEST_ASSERT_EQUAL(&at, at.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(AtSerialize, at.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(AtParse, at.atcmd.parse);
  TEST_ASSERT_EQUAL(5, at.atcmd.timeout);
}

void test_AtGetAtCommand(void)
{
  At_t at;
  AtObjectInit(&at);

  AT_Command_t *atcmd = AtGetAtCommand(&at);

  TEST_ASSERT_EQUAL(&at, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(AtSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(AtParse, atcmd->parse);
  TEST_ASSERT_EQUAL(5, atcmd->timeout);
}

void test_AtSerialize(void)
{
  At_t at;
  AtObjectInit(&at);

  char obuf[32] = {0};
  size_t n      = AtSerialize(&at, obuf, sizeof(obuf));

  TEST_ASSERT_EQUAL_STRING("AT", obuf);
}

void test_AtParse_NoEcho(void)
{
  At_t at;
  AtObjectInit(&at);

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = AtParse(&at, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, at.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AtParse_WithEcho(void)
{
  At_t at;
  AtObjectInit(&at);

  const char *ibuf = "AT\r\r\nOK\r\n";
  size_t ilen = strlen(ibuf);

  size_t n = AtParse(&at, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, at.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_AtParse_Incomplete(void)
{
  At_t at;
  AtObjectInit(&at);

  const char *ibuf = "\r\nOK\r";
  size_t length    = strlen(ibuf);

  size_t n = AtParse(&at, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, at.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_AtParse_InvalidStatus(void)
{
  At_t at;
  AtObjectInit(&at);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = AtParse(&at, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, at.response.status);
  TEST_ASSERT_EQUAL(0, n);
}
