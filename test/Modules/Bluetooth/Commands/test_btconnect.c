#include "btconnect.h"
#include "test_btconnect_static.h"
#include "unity.h"

#include <string.h>

TEST_FILE("AtCommand.c");
TEST_FILE("Utils.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_BtConnectObjectInit(void)
{
  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);

  TEST_ASSERT_EQUAL(&btconnect, btconnect.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(BtConnectSerialize, btconnect.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(BtConnectParse, btconnect.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(BtConnectTimeout, btconnect.atcmd.timeout);
  TEST_ASSERT_EQUAL(5000, btconnect.atcmd.timeoutInMilliSec);
}

void test_BtConnectSetupRequest(void)
{
  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);
  BtConnectSetupRequest(&btconnect, 3, 9);

  TEST_ASSERT_EQUAL(3, btconnect.request.deviceId);
  TEST_ASSERT_EQUAL(9, btconnect.request.profileId);
}

void test_BtConnectGetAtCommand(void)
{
  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);

  AT_Command_t *atcmd = BtConnectGetAtCommand(&btconnect);

  TEST_ASSERT_EQUAL(&btconnect, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(BtConnectSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(BtConnectParse, atcmd->parse);
  TEST_ASSERT_EQUAL(5000, atcmd->timeoutInMilliSec);
}

void test_BtConnectSerialize(void)
{
  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);
  BtConnectSetupRequest(&btconnect, 3, 9); 

  char obuf[23];
  size_t olen = sizeof(obuf);

  BtConnectSerialize(&btconnect, obuf, olen);

  TEST_ASSERT_EQUAL_STRING("AT+BTCONNECT=3,9\r", obuf);
}

void test_BtConnectParse_OK(void)
{
  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);
  BtConnectSetupRequest(&btconnect, 3, 9); 

  const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:10,\"HFP(AG)\"\r\n";
  size_t ilen = strlen(ibuf);

  BtConnectParse(&btconnect, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, btconnect.response.status);
  TEST_ASSERT_EQUAL(1, btconnect.response.result.id);
  TEST_ASSERT_EQUAL_STRING("MK-ZHANZHIMIN", btconnect.response.result.name);
  TEST_ASSERT_EQUAL_STRING("00:1a:7d:da:71:10", btconnect.response.result.address);
  TEST_ASSERT_EQUAL_STRING("HFP(AG)", btconnect.response.result.profile);
}

void test_BtConnectParse_Error(void)
{
  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);
  BtConnectSetupRequest(&btconnect, 3, 9); 

  const char *ibuf = "\r\n+BTCONNECT: 0\r\n";
  size_t ilen = strlen(ibuf);

  BtConnectParse(&btconnect, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, btconnect.response.status);
  TEST_ASSERT_EQUAL(0, btconnect.response.result.id);
  TEST_ASSERT_EQUAL_STRING("", btconnect.response.result.name);
  TEST_ASSERT_EQUAL_STRING("", btconnect.response.result.address);
  TEST_ASSERT_EQUAL_STRING("", btconnect.response.result.profile);
}

void test_BtConnectIsURC_BTCONNECTING(void)
{
    const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"SPP\"\r\n";
    size_t ilen = strlen(ibuf);

    bool result = BtConnectIsURC(ibuf, ilen);

    TEST_ASSERT(result);
}

void test_BtConnectIsURC_BTCONNECT(void)
{
    const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:10,\"HFP(AG)\"\r\n";
    size_t ilen = strlen(ibuf);

    bool result = BtConnectIsURC(ibuf, ilen);

    TEST_ASSERT(result);
}

void test_BtConnectParseURC_BTCONNECTING(void)
{
    const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"SPP\"\r\n";
    size_t ilen = strlen(ibuf);

    BtConnectURC_t urc;
    size_t n = BtConnectParseURC(&urc, ibuf, ilen);

    TEST_ASSERT_EQUAL(ilen, n);
    TEST_ASSERT_EQUAL(BTCONNECT_CONNECTING, urc.type);
    TEST_ASSERT_EQUAL_STRING("34:c7:31:aa:37:5b", urc.payload.connecting.address);
    TEST_ASSERT_EQUAL_STRING("SPP", urc.payload.connecting.profile);
}

void test_BtConnectParseURC_BTCONNECT(void)
{
    const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:10,\"HFP(AG)\"\r\n";
    size_t ilen = strlen(ibuf);

    BtConnectURC_t urc;
    size_t n = BtConnectParseURC(&urc, ibuf, ilen);

    TEST_ASSERT_EQUAL(ilen, n);
    TEST_ASSERT_EQUAL(BTCONNECT_CONNECT, urc.type);
    TEST_ASSERT_EQUAL_STRING("MK-ZHANZHIMIN", urc.payload.connect.name);
    TEST_ASSERT_EQUAL_STRING("00:1a:7d:da:71:10", urc.payload.connect.address);
    TEST_ASSERT_EQUAL_STRING("HFP(AG)", urc.payload.connect.profile);
}

void test_BtConnectParseURC_IncompleteURC(void)
{
    const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:";
    size_t ilen = strlen(ibuf);

    BtConnectURC_t urc;
    size_t n = BtConnectParseURC(&urc, ibuf, ilen);

    TEST_ASSERT_EQUAL(0, n);
    TEST_ASSERT_EQUAL(BTCONNECT_NO_URC, urc.type);
}

void test_BtConnectTimeout(void)
{
  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);

  BtConnectTimeout(&btconnect);

  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, btconnect.response.status);
}