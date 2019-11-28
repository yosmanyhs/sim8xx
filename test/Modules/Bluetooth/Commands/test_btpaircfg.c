#include "unity.h"
#include "btpaircfg.h"
#include "test_btpaircfg_static.h"

TEST_FILE("At.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_btpaircfg_ObjectInit(void)
{
    BtPaircfg_t btpaircfg;
    BtPaircfgObjectInit(&btpaircfg);

    TEST_ASSERT_EQUAL(btpaircfg.atcmd.obj, &btpaircfg);
    TEST_ASSERT_EQUAL_PTR(btpaircfg.atcmd.serialize, BtPaircfgSerialize);
    TEST_ASSERT_EQUAL_PTR(btpaircfg.atcmd.parse, BtPaircfgParse);
    TEST_ASSERT_EQUAL(btpaircfg.atcmd.timeout, 2);
}