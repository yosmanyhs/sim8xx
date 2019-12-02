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
    size_t n = BtConnectParseURC(&event.payload.btconnecting, ibuf, ilen);

    TEST_ASSERT_EQUAL(ilen, n);
    TEST_ASSERT_EQUAL_STRING("34:c7:31:aa:37:5b", event.payload.btconnecting.address);
    TEST_ASSERT_EQUAL_STRING("SPP", event.payload.btconnecting.profile);
}

void test_btconnect_URCParser_URCFollowedByAT(void)
{
    const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"SPP\"\r\n\r\nOK\r\n";
    size_t ilen = strlen(ibuf);

    GSM_BluetoothEvent_t event;
    size_t n = BtConnectParseURC(&event.payload.btconnecting, ibuf, ilen);

    TEST_ASSERT_EQUAL(44, n);
    TEST_ASSERT_EQUAL_STRING("34:c7:31:aa:37:5b", event.payload.btconnecting.address);
    TEST_ASSERT_EQUAL_STRING("SPP", event.payload.btconnecting.profile);
}

void test_btconnect_URCParser_IncompleteURC(void)
{
    const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"S";
    size_t ilen = strlen(ibuf);

    GSM_BluetoothEvent_t event;
    size_t n = BtConnectParseURC(&event.payload.btconnecting, ibuf, ilen);

    TEST_ASSERT_EQUAL(0, n);
    TEST_ASSERT_EQUAL_STRING("", event.payload.btconnecting.address);
    TEST_ASSERT_EQUAL_STRING("", event.payload.btconnecting.profile);
}