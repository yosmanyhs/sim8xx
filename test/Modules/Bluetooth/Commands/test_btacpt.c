#include "btacpt.h"
#include "test_btacpt_static.h"
#include "unity.h"

#include <string.h>
#include <stdio.h>

TEST_FILE("At.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_btacpt_ObjectInit(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);

  TEST_ASSERT_EQUAL(&btacpt, btacpt.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(BtAcptSerialize, btacpt.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(BtAcptParse, btacpt.atcmd.parse);
  TEST_ASSERT_EQUAL(5, btacpt.atcmd.timeout);
}

void test_btacpt_SetupRequest(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);
  BtAcptSetupRequest(&btacpt, 1);

  TEST_ASSERT_EQUAL(1, btacpt.request.mode);
}

void test_btacpt_GetAtCommand(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);

  AT_Command_t *atcmd = BtAcptGetAtCommand(&btacpt);

  TEST_ASSERT_EQUAL(&btacpt, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(BtAcptSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(BtAcptParse, atcmd->parse);
  TEST_ASSERT_EQUAL(5, atcmd->timeout);
}

void test_btacpt_Serialize(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);
  BtAcptSetupRequest(&btacpt, 1);

  char obuf[32] = {0};
  size_t n      = BtAcptSerialize(&btacpt, obuf, sizeof(obuf));

  TEST_ASSERT_EQUAL_STRING("AT+BTACPT=1", obuf);
}

void test_btacpt_Parse(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);

  const char *ibuf = "\r\nOK\r\n";
  size_t length    = strlen(ibuf);

  size_t n = BtAcptParse(&btacpt, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_OK, btacpt.response.status);
  TEST_ASSERT_EQUAL(6, n);
}

void test_btacpt_Parse_Incomplete(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);

  const char *ibuf = "\r\nOK\r";
  size_t length    = strlen(ibuf);

  size_t n = BtAcptParse(&btacpt, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btacpt.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_btacpt_Parse_InvalidStatus(void)
{
  BtAcpt_t btacpt;
  BtAcptObjectInit(&btacpt);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = BtAcptParse(&btacpt, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btacpt.response.status);
  TEST_ASSERT_EQUAL(0, n);
}
