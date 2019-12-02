#include "btsppsend.h"
#include "test_btsppsend_static.h"
#include "unity.h"

#include <string.h>

TEST_FILE("At.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_btsppsend_ObjectInit(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);

  TEST_ASSERT_EQUAL(btsppsend.atcmd.obj, &btsppsend);
  TEST_ASSERT_EQUAL_PTR(btsppsend.atcmd.serialize, BtSppSendSerialize);
  TEST_ASSERT_EQUAL_PTR(btsppsend.atcmd.parse, BtSppSendParse);
  TEST_ASSERT_EQUAL(10, btsppsend.atcmd.timeout);
}

void test_btsppsend_BtSppSendSetCommandMode(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);

  BtSppSendSetCommandMode(&btsppsend);

  TEST_ASSERT_EQUAL(BT_SPP_SEND_STATE_COMMAND, btsppsend.state);
}

void test_btsppsend_BtSppSendSetDataMode(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);

  BtSppSendSetDataMode(&btsppsend);

  TEST_ASSERT_EQUAL(BT_SPP_SEND_STATE_DATA, btsppsend.state);
}

void test_btsppsend_BtSppSendSetupRequest(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);

  const char *data = "Test SPP message.";
  size_t length    = strlen(data);
  BtSppSendSetupRequest(&btsppsend, data, length);

  TEST_ASSERT_EQUAL(data, btsppsend.request.data);
  TEST_ASSERT_EQUAL(length, btsppsend.request.length);
}

void test_btsppsend_BtSppSendSerialize_CommandMode(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);

  const char *data = "Test SPP message.";
  size_t length    = strlen(data);
  BtSppSendSetCommandMode(&btsppsend);
  BtSppSendSetupRequest(&btsppsend, data, length);

  char obuf[32] = {0};
  size_t n      = BtSppSendSerialize(&btsppsend, obuf, sizeof(obuf));

  TEST_ASSERT_EQUAL_STRING("AT+BTSPPSEND", obuf);
}

void test_btsppsend_BtSppSendSerialize_DataMode(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);

  const char *data = "Test SPP message.";
  size_t length    = strlen(data);
  BtSppSendSetDataMode(&btsppsend);
  BtSppSendSetupRequest(&btsppsend, data, length);

  char obuf[32] = {0};
  size_t n      = BtSppSendSerialize(&btsppsend, obuf, sizeof(obuf));

  TEST_ASSERT_EQUAL_STRING(data, obuf);
}

void test_btsppsend_Parse(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);

  const char *ibuf = "\r\nOK\r\n=CGNSINF...";
  size_t length    = strlen(ibuf);

  size_t n = BtSppSendParse(&btsppsend, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btsppsend.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_btsppsend_Parse_Incomplete(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);

  const char *ibuf = "\r\nOK\r";
  size_t length    = strlen(ibuf);

  size_t n = BtSppSendParse(&btsppsend, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btsppsend.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_btsppsend_Parse_InvalidStatus(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = BtSppSendParse(&btsppsend, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btsppsend.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_btsppsend_Parse_WAIT_USER_DATA(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);

  const char *ibuf = "\r\n> \r\n.";
  size_t length    = strlen(ibuf);

  size_t n = BtSppSendParse(&btsppsend, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_WAIT_FOR_USER_DATA, btsppsend.response.status);
  TEST_ASSERT_EQUAL(2, n);
}

void test_btsppsend_Parse_SEND_OK(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);
  BtSppSendSetDataMode(&btsppsend);

  const char *ibuf = "\r\nSEND OK\r\n.";
  size_t length    = strlen(ibuf);

  size_t n = BtSppSendParse(&btsppsend, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_SEND_OK, btsppsend.response.status);
  TEST_ASSERT_EQUAL(11, n);
}

void test_btsppsend_Parse_SEND_FAIL(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);
  BtSppSendSetDataMode(&btsppsend);

  const char *ibuf = "\r\nSEND FAIL\r\n.";
  size_t length    = strlen(ibuf);

  size_t n = BtSppSendParse(&btsppsend, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_SEND_FAIL, btsppsend.response.status);
  TEST_ASSERT_EQUAL(13, n);
}

void test_btsppsend_Parse_ERROR(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);
  BtSppSendSetDataMode(&btsppsend);

  const char *ibuf = "\r\nERROR\r\n.";
  size_t length    = strlen(ibuf);

  size_t n = BtSppSendParse(&btsppsend, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, btsppsend.response.status);
  TEST_ASSERT_EQUAL(9, n);
}

void test_btsppsend_Parse_Unexpected_Status(void)
{
  BtSppSend_t btsppsend;
  BtSppSendObjectInit(&btsppsend);
  BtSppSendSetDataMode(&btsppsend);

  const char *ibuf = "\r\nPROCEEDING\r\n.";
  size_t length    = strlen(ibuf);

  size_t n = BtSppSendParse(&btsppsend, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, btsppsend.response.status);
  TEST_ASSERT_EQUAL(0, n);
}