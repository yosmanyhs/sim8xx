#include "btconnect.h"
#include "unity.h"

#include "Bluetooth.h"

#include <string.h>

TEST_FILE("Utils.c");
TEST_FILE("btpower.c");
TEST_FILE("bthost.c");
TEST_FILE("btsppsend.c");
TEST_FILE("btpaircfg.c");
TEST_FILE("Modem.c");
TEST_FILE("At.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_btconnect_URCParser(void)
{
    const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"SPP\"\r\n";
    size_t ilen = strlen(ibuf);

    GSM_BluetoothEvent_t event;
    size_t n = BtConnectParseURC_BtConnecting(&event.payload.btconnecting, ibuf, ilen);

    TEST_ASSERT_EQUAL(ilen, n);
    TEST_ASSERT_EQUAL_STRING("34:c7:31:aa:37:5b", event.payload.btconnecting.address);
    TEST_ASSERT_EQUAL_STRING("SPP", event.payload.btconnecting.profile);
}

void test_btconnect_URCParser_URCFollowedByAT(void)
{
    const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"SPP\"\r\n\r\nOK\r\n";
    size_t ilen = strlen(ibuf);

    GSM_BluetoothEvent_t event;
    size_t n = BtConnectParseURC_BtConnecting(&event.payload.btconnecting, ibuf, ilen);

    TEST_ASSERT_EQUAL(44, n);
    TEST_ASSERT_EQUAL_STRING("34:c7:31:aa:37:5b", event.payload.btconnecting.address);
    TEST_ASSERT_EQUAL_STRING("SPP", event.payload.btconnecting.profile);
}

void test_btconnect_URCParser_IncompleteURC(void)
{
    const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"S";
    size_t ilen = strlen(ibuf);

    GSM_BluetoothEvent_t event;
    size_t n = BtConnectParseURC_BtConnecting(&event.payload.btconnecting, ibuf, ilen);

    TEST_ASSERT_EQUAL(0, n);
    TEST_ASSERT_EQUAL_STRING("", event.payload.btconnecting.address);
    TEST_ASSERT_EQUAL_STRING("", event.payload.btconnecting.profile);
}

void test_btconnect_BTCONNECT_URCParser(void)
{
    const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:10,\"HFP(AG)\"\r\n";
    size_t ilen = strlen(ibuf);

    GSM_BluetoothEvent_t event;
    size_t n = BtConnectParseURC_BtConnect(&event.payload.btconnect, ibuf, ilen);

    TEST_ASSERT_EQUAL(ilen, n);
    TEST_ASSERT_EQUAL(1, event.payload.btconnect.id);
    TEST_ASSERT_EQUAL_STRING("MK-ZHANZHIMIN", event.payload.btconnect.name);
    TEST_ASSERT_EQUAL_STRING("00:1a:7d:da:71:10", event.payload.btconnect.address);
    TEST_ASSERT_EQUAL_STRING("HFP(AG)", event.payload.btconnect.profile);
}