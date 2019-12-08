#include "btdisconn.h"
#include "test_btdisconn_static.h"
#include "unity.h"

#include <string.h>

TEST_FILE("At.c");
TEST_FILE("Utils.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_BtDisconnIsURC(void)
{
    const char *ibuf = "\r\n+BTDISCONN:\"SIM800H...";
    size_t ilen = strlen(ibuf);

    bool result = BtDisconnIsURC(ibuf, ilen);

    TEST_ASSERT(result);
}

void test_BtDisconnParseURC_BTDISCONN(void)
{
    const char *ibuf = "\r\n+BTDISCONN:\"SIM800H\",34:c7:31:aa:37:5b,\"SPP\"\r\n";
    size_t ilen = strlen(ibuf);

    BtDisconnURC_t urc;
    size_t n = BtDisconnParseURC(&urc, ibuf, ilen);

    TEST_ASSERT_EQUAL(ilen, n);
    TEST_ASSERT_EQUAL(BTDISCONN_DISCONNECTED, urc.type);
    TEST_ASSERT_EQUAL_STRING("SIM800H", urc.payload.disconn.name);
    TEST_ASSERT_EQUAL_STRING("34:c7:31:aa:37:5b", urc.payload.disconn.address);
    TEST_ASSERT_EQUAL_STRING("SPP", urc.payload.disconn.profile);
}

void test_BtDisconnParseURC_IncompleteURC(void)
{
    const char *ibuf = "\r\n+BTDISCONN:\"SIM800H\",34:c7:31:aa:37:5b,\"SPP";
    size_t ilen = strlen(ibuf);

    BtDisconnURC_t urc;
    size_t n = BtDisconnParseURC(&urc, ibuf, ilen);

    TEST_ASSERT_EQUAL(0, n);
    TEST_ASSERT_EQUAL(BTDISCONN_NO_URC, urc.type);
    TEST_ASSERT_EQUAL_STRING("", urc.payload.disconn.name);
    TEST_ASSERT_EQUAL_STRING("", urc.payload.disconn.address);
    TEST_ASSERT_EQUAL_STRING("", urc.payload.disconn.profile);
}
