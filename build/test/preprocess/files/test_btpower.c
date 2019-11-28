#include "@@@@At.c"
#include "source/Modem/At.c"
#include "build/temp/_test_btpower.c"
#include "test_btpower_static.h"
#include "btpower.h"
#include "unity.h"






                ;



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



    UnityAssertEqualNumber((UNITY_INT)((btpower.atcmd.obj)), (UNITY_INT)((&btpower)), (

   ((void *)0)

   ), (UNITY_UINT)(22), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT64)((btpower.atcmd.serialize)), (UNITY_INT64)((BtPowerSerialize)), (

   ((void *)0)

   ), (UNITY_UINT)(23), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((btpower.atcmd.parse)), (UNITY_INT64)((BtPowerParse)), (

   ((void *)0)

   ), (UNITY_UINT)(24), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT)((btpower.atcmd.timeout)), (UNITY_INT)((10)), (

   ((void *)0)

   ), (UNITY_UINT)(25), UNITY_DISPLAY_STYLE_INT);

}



void test_btpower_SetupRequest_Mode0(void)

{

    BtPower_t btpower;

    BtPowerObjectInit(&btpower);

    BtPowerSetupRequest(&btpower, 0);



    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((btpower.request.mode)), (

   ((void *)0)

   ), (UNITY_UINT)(34), UNITY_DISPLAY_STYLE_INT);

}



void test_btpower_SetupRequest_Mode1(void)

{

    BtPower_t btpower;

    BtPowerObjectInit(&btpower);

    BtPowerSetupRequest(&btpower, 1);



    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((btpower.request.mode)), (

   ((void *)0)

   ), (UNITY_UINT)(43), UNITY_DISPLAY_STYLE_INT);

}



void test_btpower_GetAtCommand(void)

{

    BtPower_t btpower;

    BtPowerObjectInit(&btpower);



    AT_Command_t *atcmd = BtPowerGetAtCommand(&btpower);



    UnityAssertEqualNumber((UNITY_INT)((atcmd->obj)), (UNITY_INT)((&btpower)), (

   ((void *)0)

   ), (UNITY_UINT)(53), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT64)((atcmd->serialize)), (UNITY_INT64)((BtPowerSerialize)), (

   ((void *)0)

   ), (UNITY_UINT)(54), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((atcmd->parse)), (UNITY_INT64)((BtPowerParse)), (

   ((void *)0)

   ), (UNITY_UINT)(55), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT)((atcmd->timeout)), (UNITY_INT)((10)), (

   ((void *)0)

   ), (UNITY_UINT)(56), UNITY_DISPLAY_STYLE_INT);

}



void test_btpower_Serialize_Mode0(void)

{

    BtPower_t btpower;

    BtPowerObjectInit(&btpower);

    BtPowerSetupRequest(&btpower, 0);



    char obuf[32] = {0};

    size_t n = BtPowerSerialize(&btpower, obuf, sizeof(obuf));



    UnityAssertEqualString((const char*)(("AT+BTPOWER=0")), (const char*)((obuf)), (

   ((void *)0)

   ), (UNITY_UINT)(68));

}



void test_btpower_Serialize_Mode1(void)

{

    BtPower_t btpower;

    BtPowerObjectInit(&btpower);

    BtPowerSetupRequest(&btpower, 1);



    char obuf[32] = {0};

    size_t n = BtPowerSerialize(&btpower, obuf, sizeof(obuf));



    UnityAssertEqualString((const char*)(("AT+BTPOWER=1")), (const char*)((obuf)), (

   ((void *)0)

   ), (UNITY_UINT)(80));

}



void test_btpower_Parse(void)

{

    BtPower_t btpower;

    BtPowerObjectInit(&btpower);

    BtPowerSetupRequest(&btpower, 1);



    const char *ibuf = "\r\nOK\r\n=CGNSINF...";

    size_t length = strlen(ibuf);



    size_t n = BtPowerParse(&btpower, ibuf, length);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_OK)), (UNITY_INT)((btpower.response.status)), (

   ((void *)0)

   ), (UNITY_UINT)(94), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((6)), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(95), UNITY_DISPLAY_STYLE_INT);

}



void test_btpower_Parse_Incomplete(void)

{

    BtPower_t btpower;

    BtPowerObjectInit(&btpower);

    BtPowerSetupRequest(&btpower, 1);



    const char *ibuf = "\r\nOK\r";

    size_t length = strlen(ibuf);



    size_t n = BtPowerParse(&btpower, ibuf, length);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_INVALID)), (UNITY_INT)((btpower.response.status)), (

   ((void *)0)

   ), (UNITY_UINT)(109), UNITY_DISPLAY_STYLE_INT);

}



void test_btpower_Parse_Incomplete_length(void)

{

    BtPower_t btpower;

    BtPowerObjectInit(&btpower);

    BtPowerSetupRequest(&btpower, 1);



    const char *ibuf = "\r\nOK\r";

    size_t length = strlen(ibuf);



    size_t n = BtPowerParse(&btpower, ibuf, length);



    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(123), UNITY_DISPLAY_STYLE_INT);

}



void test_btpower_Parse_InvalidStatus(void)

{

    BtPower_t btpower;

    BtPowerObjectInit(&btpower);

    BtPowerSetupRequest(&btpower, 1);



    const char *ibuf = "\r\nPROCEEDING\r\n";

    size_t length = strlen(ibuf);



    size_t n = BtPowerParse(&btpower, ibuf, length);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_INVALID)), (UNITY_INT)((btpower.response.status)), (

   ((void *)0)

   ), (UNITY_UINT)(137), UNITY_DISPLAY_STYLE_INT);

}



void test_btpower_Parse_InvalidStatus_length(void)

{

    BtPower_t btpower;

    BtPowerObjectInit(&btpower);

    BtPowerSetupRequest(&btpower, 1);



    const char *ibuf = "\r\nPROCEEDING\r\n";

    size_t length = strlen(ibuf);



    size_t n = BtPowerParse(&btpower, ibuf, length);



    UnityAssertEqualNumber((UNITY_INT)((6)), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(151), UNITY_DISPLAY_STYLE_INT);

}
