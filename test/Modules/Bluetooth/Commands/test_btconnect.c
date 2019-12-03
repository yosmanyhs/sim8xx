#include "btconnect.h"
#include "test_btconnect_static.h"
#include "unity.h"

#include "Bluetooth.h"

#include <string.h>

TEST_FILE("btpower.c");
TEST_FILE("bthost.c");
TEST_FILE("btsppsend.c");
TEST_FILE("btpaircfg.c");
TEST_FILE("Modem.c");
TEST_FILE("At.c");
TEST_FILE("Utils.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_btconnect_ObjectInit(void)
{
  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);

  TEST_ASSERT_EQUAL(&btconnect, btconnect.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(BtConnectSerialize, btconnect.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(BtConnectParse, btconnect.atcmd.parse);
  TEST_ASSERT_EQUAL(5, btconnect.atcmd.timeout);
}

void test_btconnect_SetupRequest(void)
{
  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);
  BtConnectSetupRequest(&btconnect, 3, 9);

  TEST_ASSERT_EQUAL(3, btconnect.request.deviceId);
  TEST_ASSERT_EQUAL(9, btconnect.request.profileId);
}

void test_btconnect_GetAtCommand(void)
{
  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);

  AT_Command_t *atcmd = BtConnectGetAtCommand(&btconnect);

  TEST_ASSERT_EQUAL(&btconnect, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(BtConnectSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(BtConnectParse, atcmd->parse);
  TEST_ASSERT_EQUAL(5, atcmd->timeout);
}

void test_btconnect_Serialize(void)
{
  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);
  BtConnectSetupRequest(&btconnect, 3, 9); 

  char obuf[20];
  size_t olen = sizeof(obuf);

  BtConnectSerialize(&btconnect, obuf, olen);

  TEST_ASSERT_EQUAL_STRING("AT+BTCONNECT=3,9", obuf);
}

void test_btconnect_Parse_OK(void)
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

void test_btconnect_Parse_Error(void)
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

void test_btconnect_IsURC_BTCONNECTING(void)
{
    const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"SPP\"\r\n";
    size_t ilen = strlen(ibuf);

    bool result = BtConnectIsURC(ibuf, ilen);

    TEST_ASSERT(result);
}

void test_btconnect_IsURC_BTCONNECT(void)
{
    const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:10,\"HFP(AG)\"\r\n";
    size_t ilen = strlen(ibuf);

    bool result = BtConnectIsURC(ibuf, ilen);

    TEST_ASSERT(result);
}

void test_btconnect_ParseURC_BTCONNECTING(void)
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

void test_btconnect_ParseURC_BTCONNECT(void)
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

void test_btconnect_URCParser_IncompleteURC(void)
{
    const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:";
    size_t ilen = strlen(ibuf);

    BtConnectURC_t urc;
    size_t n = BtConnectParseURC(&urc, ibuf, ilen);

    TEST_ASSERT_EQUAL(0, n);
    TEST_ASSERT_EQUAL(BTCONNECT_NO_URC, urc.type);
}
