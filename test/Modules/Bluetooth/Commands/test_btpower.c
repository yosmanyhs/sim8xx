#include "btpower.h"
#include "test_btpower_static.h"
#include "unity.h"

#include <string.h>

TEST_FILE("AtCommand.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_BtPowerObjectInit(void)
{
  BtPower_t btpower;
  BtPowerObjectInit(&btpower);

  TEST_ASSERT_EQUAL(&btpower, btpower.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(BtPowerSerialize, btpower.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(BtPowerParse, btpower.atcmd.parse);
  TEST_ASSERT_EQUAL(10, btpower.atcmd.timeout);
}

void test_BtPowerSetupRequest_Mode0(void)
{
  BtPower_t btpower;
  BtPowerObjectInit(&btpower);
  BtPowerSetupRequest(&btpower, 0);

  TEST_ASSERT_EQUAL(0, btpower.request.mode);
}

void test_BtPowerSetupRequest_Mode1(void)
{
  BtPower_t btpower;
  BtPowerObjectInit(&btpower);
  BtPowerSetupRequest(&btpower, 1);

  TEST_ASSERT_EQUAL(1, btpower.request.mode);
}

void test_BtPowerGetAtCommand(void)
{
  BtPower_t btpower;
  BtPowerObjectInit(&btpower);

  AT_Command_t *atcmd = BtPowerGetAtCommand(&btpower);

  TEST_ASSERT_EQUAL(&btpower, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(BtPowerSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(BtPowerParse, atcmd->parse);
  TEST_ASSERT_EQUAL(10, atcmd->timeout);
}

void test_BtPowerSerialize_Mode0(void)
{
  BtPower_t btpower;
  BtPowerObjectInit(&btpower);
  BtPowerSetupRequest(&btpower, 0);

  char obuf[32] = {0};
  size_t n      = BtPowerSerialize(&btpower, obuf, sizeof(obuf));

  TEST_ASSERT_EQUAL_STRING("AT+BTPOWER=0", obuf);
}

void test_BtPowerSerialize_Mode1(void)
{
  BtPower_t btpower;
  BtPowerObjectInit(&btpower);
  BtPowerSetupRequest(&btpower, 1);

  char obuf[32] = {0};
  size_t n      = BtPowerSerialize(&btpower, obuf, sizeof(obuf));

  TEST_ASSERT_EQUAL_STRING("AT+BTPOWER=1", obuf);
}

void test_BtPowerParse(void)
{
  BtPower_t btpower;
  BtPowerObjectInit(&btpower);

  const char *ibuf = "\r\nOK\r\n=CGNSINF...";
  size_t length    = strlen(ibuf);

  size_t n = BtPowerParse(&btpower, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_OK, btpower.response.status);
  TEST_ASSERT_EQUAL(6, n);
}

void test_BtPowerParse_Incomplete(void)
{
  BtPower_t btpower;
  BtPowerObjectInit(&btpower);

  const char *ibuf = "\r\nOK\r";
  size_t length    = strlen(ibuf);

  size_t n = BtPowerParse(&btpower, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btpower.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_BtPowerParse_InvalidStatus(void)
{
  BtPower_t btpower;
  BtPowerObjectInit(&btpower);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = BtPowerParse(&btpower, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btpower.response.status);
  TEST_ASSERT_EQUAL(0, n);
}
