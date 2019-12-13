#include "btacpt.h"
#include "test_btacpt_static.h"
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

void test_BtAcptObjectInit(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);

  TEST_ASSERT_EQUAL(&btacpt, btacpt.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(BtAcptSerialize, btacpt.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(BtAcptParse, btacpt.atcmd.parse);
  TEST_ASSERT_EQUAL(5, btacpt.atcmd.timeout);
}

void test_BtAcptSetupRequest(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);
  BtAcptSetupRequest(&btacpt, 1);

  TEST_ASSERT_EQUAL(1, btacpt.request.mode);
}

void test_BtAcptGetAtCommand(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);

  AT_Command_t *atcmd = BtAcptGetAtCommand(&btacpt);

  TEST_ASSERT_EQUAL(&btacpt, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(BtAcptSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(BtAcptParse, atcmd->parse);
  TEST_ASSERT_EQUAL(5, atcmd->timeout);
}

void test_BtAcptSerialize_Mode0(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);
  BtAcptSetupRequest(&btacpt, 0);

  char obuf[32] = {0};
  size_t n      = BtAcptSerialize(&btacpt, obuf, sizeof(obuf));

  TEST_ASSERT_EQUAL_STRING("AT+BTACPT=0", obuf);
}

void test_BtAcptSerialize_Mode1(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);
  BtAcptSetupRequest(&btacpt, 1);

  char obuf[32] = {0};
  size_t n      = BtAcptSerialize(&btacpt, obuf, sizeof(obuf));

  TEST_ASSERT_EQUAL_STRING("AT+BTACPT=1", obuf);
}

void test_BtAcptParse(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);

  const char *ibuf = "\r\nOK\r\n";
  size_t length    = strlen(ibuf);

  size_t n = BtAcptParse(&btacpt, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_OK, btacpt.response.status);
  TEST_ASSERT_EQUAL(6, n);
}

void test_BtAcptParse_Incomplete(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);

  const char *ibuf = "\r\nOK\r";
  size_t length    = strlen(ibuf);

  size_t n = BtAcptParse(&btacpt, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btacpt.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_BtAcptParse_InvalidStatus(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = BtAcptParse(&btacpt, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btacpt.response.status);
  TEST_ASSERT_EQUAL(0, n);
}
