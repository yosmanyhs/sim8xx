#include "cgnspwr.h"
#include "test_cgnspwr_static.h"
#include "unity.h"

#include <string.h>

TEST_FILE("At.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_CgnsPwrObjectInit(void)
{
  CgnsPwr_t btpower;
  CgnsPwrObjectInit(&btpower);

  TEST_ASSERT_EQUAL(&btpower, btpower.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CgnsPwrSerialize, btpower.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CgnsPwrParse, btpower.atcmd.parse);
  TEST_ASSERT_EQUAL(10, btpower.atcmd.timeout);
}

void test_CgnsPwrSetupRequest_Mode0(void)
{
  CgnsPwr_t btpower;
  CgnsPwrObjectInit(&btpower);
  CgnsPwrSetupRequest(&btpower, 0);

  TEST_ASSERT_EQUAL(0, btpower.request.mode);
}

void test_CgnsPwrSetupRequest_Mode1(void)
{
  CgnsPwr_t btpower;
  CgnsPwrObjectInit(&btpower);
  CgnsPwrSetupRequest(&btpower, 1);

  TEST_ASSERT_EQUAL(1, btpower.request.mode);
}

void test_CgnsPwrGetAtCommand(void)
{
  CgnsPwr_t btpower;
  CgnsPwrObjectInit(&btpower);

  AT_Command_t *atcmd = CgnsPwrGetAtCommand(&btpower);

  TEST_ASSERT_EQUAL(&btpower, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CgnsPwrSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CgnsPwrParse, atcmd->parse);
  TEST_ASSERT_EQUAL(10, atcmd->timeout);
}

void test_CgnsPwrSerialize_Mode0(void)
{
  CgnsPwr_t btpower;
  CgnsPwrObjectInit(&btpower);
  CgnsPwrSetupRequest(&btpower, 0);

  char obuf[32] = {0};
  size_t n      = CgnsPwrSerialize(&btpower, obuf, sizeof(obuf));

  TEST_ASSERT_EQUAL_STRING("AT+CGNSPWR=0", obuf);
}

void test_CgnsPwrSerialize_Mode1(void)
{
  CgnsPwr_t btpower;
  CgnsPwrObjectInit(&btpower);
  CgnsPwrSetupRequest(&btpower, 1);

  char obuf[32] = {0};
  size_t n      = CgnsPwrSerialize(&btpower, obuf, sizeof(obuf));

  TEST_ASSERT_EQUAL_STRING("AT+CGNSPWR=1", obuf);
}

void test_CgnsPwrParse(void)
{
  CgnsPwr_t btpower;
  CgnsPwrObjectInit(&btpower);

  const char *ibuf = "\r\nOK\r\n=CGNSINF...";
  size_t length    = strlen(ibuf);

  size_t n = CgnsPwrParse(&btpower, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_OK, btpower.response.status);
  TEST_ASSERT_EQUAL(6, n);
}

void test_CgnsPwrParse_Incomplete(void)
{
  CgnsPwr_t btpower;
  CgnsPwrObjectInit(&btpower);

  const char *ibuf = "\r\nOK\r";
  size_t length    = strlen(ibuf);

  size_t n = CgnsPwrParse(&btpower, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btpower.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CgnsPwrParse_InvalidStatus(void)
{
  CgnsPwr_t btpower;
  CgnsPwrObjectInit(&btpower);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CgnsPwrParse(&btpower, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btpower.response.status);
  TEST_ASSERT_EQUAL(0, n);
}
