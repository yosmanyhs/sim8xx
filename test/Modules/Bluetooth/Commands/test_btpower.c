#include "unity.h"
#include "btpower.h"
#include "test_btpower_static.h"

#include <string.h>

TEST_FILE("At.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_btpower_ObjectInit(void)
{
    BtPower_t btpower;
    BtPowerObjectInit(&btpower);

    TEST_ASSERT_EQUAL(btpower.atcmd.obj, &btpower);
    TEST_ASSERT_EQUAL_PTR(btpower.atcmd.serialize, BtPowerSerialize);
    TEST_ASSERT_EQUAL_PTR(btpower.atcmd.parse, BtPowerParse);
    TEST_ASSERT_EQUAL(btpower.atcmd.timeout, 10);
}

void test_btpower_SetupRequest_Mode0(void)
{
    BtPower_t btpower;
    BtPowerObjectInit(&btpower);
    BtPowerSetupRequest(&btpower, 0);

    TEST_ASSERT_EQUAL(0, btpower.request.mode);
}

void test_btpower_SetupRequest_Mode1(void)
{
    BtPower_t btpower;
    BtPowerObjectInit(&btpower);
    BtPowerSetupRequest(&btpower, 1);

    TEST_ASSERT_EQUAL(1, btpower.request.mode);
}

void test_btpower_GetAtCommand(void)
{
    BtPower_t btpower;
    BtPowerObjectInit(&btpower);
    
    AT_Command_t *atcmd = BtPowerGetAtCommand(&btpower);

    TEST_ASSERT_EQUAL(atcmd->obj, &btpower);
    TEST_ASSERT_EQUAL_PTR(atcmd->serialize, BtPowerSerialize);
    TEST_ASSERT_EQUAL_PTR(atcmd->parse, BtPowerParse);
    TEST_ASSERT_EQUAL(atcmd->timeout, 10);
}

void test_btpower_Serialize_Mode0(void)
{
    BtPower_t btpower;
    BtPowerObjectInit(&btpower);
    BtPowerSetupRequest(&btpower, 0);

    char obuf[32] = {0};
    size_t n = BtPowerSerialize(&btpower, obuf, sizeof(obuf));

    TEST_ASSERT_EQUAL_STRING("AT+BTPOWER=0", obuf);
}

void test_btpower_Serialize_Mode1(void)
{
    BtPower_t btpower;
    BtPowerObjectInit(&btpower);
    BtPowerSetupRequest(&btpower, 1);

    char obuf[32] = {0};
    size_t n = BtPowerSerialize(&btpower, obuf, sizeof(obuf));

    TEST_ASSERT_EQUAL_STRING("AT+BTPOWER=1", obuf);
}

void test_btpower_Parse(void)
{
    BtPower_t btpower;
    BtPowerObjectInit(&btpower);
    BtPowerSetupRequest(&btpower, 1);

    const char *ibuf = "\r\nOK\r\n=CGNSINF...";
    size_t length = strlen(ibuf);

    size_t n = BtPowerParse(&btpower, ibuf, length);

    TEST_ASSERT_EQUAL(AT_CMD_OK, btpower.response.status);
    TEST_ASSERT_EQUAL(6, n);
}

void test_btpower_Parse_Incomplete(void)
{
    BtPower_t btpower;
    BtPowerObjectInit(&btpower);
    BtPowerSetupRequest(&btpower, 1);

    const char *ibuf = "\r\nOK\r";
    size_t length = strlen(ibuf);

    size_t n = BtPowerParse(&btpower, ibuf, length);

    TEST_ASSERT_EQUAL(AT_CMD_INVALID, btpower.response.status);
}

void test_btpower_Parse_Incomplete_length(void)
{
    BtPower_t btpower;
    BtPowerObjectInit(&btpower);
    BtPowerSetupRequest(&btpower, 1);

    const char *ibuf = "\r\nOK\r";
    size_t length = strlen(ibuf);

    size_t n = BtPowerParse(&btpower, ibuf, length);

    TEST_ASSERT_EQUAL(0, n);
}

void test_btpower_Parse_InvalidStatus(void)
{
    BtPower_t btpower;
    BtPowerObjectInit(&btpower);
    BtPowerSetupRequest(&btpower, 1);

    const char *ibuf = "\r\nPROCEEDING\r\n";
    size_t length = strlen(ibuf);

    size_t n = BtPowerParse(&btpower, ibuf, length);

    TEST_ASSERT_EQUAL(AT_CMD_INVALID, btpower.response.status);
}

void test_btpower_Parse_InvalidStatus_length(void)
{
    BtPower_t btpower;
    BtPowerObjectInit(&btpower);
    BtPowerSetupRequest(&btpower, 1);

    const char *ibuf = "\r\nPROCEEDING\r\n";
    size_t length = strlen(ibuf);

    size_t n = BtPowerParse(&btpower, ibuf, length);

    TEST_ASSERT_EQUAL(0, n);
}
