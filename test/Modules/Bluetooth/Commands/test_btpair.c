#include "btpair.h"
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

void test_BtPairIsURC(void)
{
    const char *ibuf = "\r\n+BTPAIR: 1,\"Redmi-Z\",00:ec:0a:66:10:cf\r\n";
    size_t ilen = strlen(ibuf);

    bool result = BtPairIsURC(ibuf, ilen);

    TEST_ASSERT(result);
}

void test_BtPairParseURC(void)
{
    const char *ibuf = "\r\n+BTPAIR: 1,\"Redmi-Z\",00:ec:0a:66:10:cf\r\n";
    size_t ilen = strlen(ibuf);

    BtPairURC_t urc;
    size_t n = BtPairParseURC(&urc, ibuf, ilen);

    TEST_ASSERT_EQUAL(ilen, n);
    TEST_ASSERT_EQUAL(BTPAIR_PASSIVE_MODE_WITH_SUCCESS, urc.type);
    TEST_ASSERT_EQUAL(1, urc.payload.passiveSuccess.id);
    TEST_ASSERT_EQUAL_STRING("Redmi-Z", urc.payload.passiveSuccess.name);
    TEST_ASSERT_EQUAL_STRING("00:ec:0a:66:10:cf", urc.payload.passiveSuccess.address);
}

void test_btpairParseURC_IncompleteURC(void)
{
    const char *ibuf = "\r\n+BTPAIR: 1,\"Redmi-Z\",00:ec:0a:66";
    size_t ilen = strlen(ibuf);

    BtPairURC_t urc;
    size_t n = BtPairParseURC(&urc, ibuf, ilen);

    TEST_ASSERT_EQUAL(0, n);
    TEST_ASSERT_EQUAL(BTPAIR_NO_URC, urc.type);
    TEST_ASSERT_EQUAL(0, urc.payload.passiveSuccess.id);
    TEST_ASSERT_EQUAL_STRING("", urc.payload.passiveSuccess.name);
    TEST_ASSERT_EQUAL_STRING("", urc.payload.passiveSuccess.address);
}
