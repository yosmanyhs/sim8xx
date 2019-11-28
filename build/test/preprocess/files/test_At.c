#include "build/temp/_test_At.c"
#include "At.h"
#include "unity.h"






void setUp(void)

{

}



void tearDown(void)

{

}



void test_At_Parse_OK(void)

{

    const char *input = "\r\nOK\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_OK)), (UNITY_INT)((status)), (

   ((void *)0)

   ), (UNITY_UINT)(21), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_OK_length(void)

{

    const char *input = "\r\nOK\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((strlen(input))), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(31), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_OK_Incomplete(void)

{

    const char *input = "\r\nOK\r";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_INVALID)), (UNITY_INT)((status)), (

   ((void *)0)

   ), (UNITY_UINT)(41), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(42), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_OK_Incomplete_length(void)

{

    const char *input = "\r\nOK";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(52), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_CONNECT(void)

{

    const char *input = "\r\nCONNECT\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_CONNECT)), (UNITY_INT)((status)), (

   ((void *)0)

   ), (UNITY_UINT)(62), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_CONNECT_length(void)

{

    const char *input = "\r\nCONNECT\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((strlen(input))), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(72), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_RING(void)

{

    const char *input = "\r\nRING\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_RING)), (UNITY_INT)((status)), (

   ((void *)0)

   ), (UNITY_UINT)(82), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_RING_length(void)

{

    const char *input = "\r\nRING\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((strlen(input))), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(92), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_NO_CARRIER(void)

{

    const char *input = "\r\nNO CARRIER\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_NO_CARRIER)), (UNITY_INT)((status)), (

   ((void *)0)

   ), (UNITY_UINT)(102), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_NO_CARRIER_length(void)

{

    const char *input = "\r\nNO CARRIER\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((strlen(input))), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(112), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_ERROR(void)

{

    const char *input = "\r\nERROR\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_ERROR)), (UNITY_INT)((status)), (

   ((void *)0)

   ), (UNITY_UINT)(122), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_ERROR_length(void)

{

    const char *input = "\r\nERROR\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((strlen(input))), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(132), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_NO_DIALTONE(void)

{

    const char *input = "\r\nNO DIALTONE\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_NO_DIALTONE)), (UNITY_INT)((status)), (

   ((void *)0)

   ), (UNITY_UINT)(142), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_NO_DIALTONE_length(void)

{

    const char *input = "\r\nNO DIALTONE\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((strlen(input))), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(152), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_BUSY(void)

{

    const char *input = "\r\nBUSY\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_BUSY)), (UNITY_INT)((status)), (

   ((void *)0)

   ), (UNITY_UINT)(162), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_BUSY_length(void)

{

    const char *input = "\r\nBUSY\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((strlen(input))), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(172), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_NO_ANSWER(void)

{

    const char *input = "\r\nNO ANSWER\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_NO_ANSWER)), (UNITY_INT)((status)), (

   ((void *)0)

   ), (UNITY_UINT)(182), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_NO_ANSWER_length(void)

{

    const char *input = "\r\nNO ANSWER\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((strlen(input))), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(192), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_PROCEEDING(void)

{

    const char *input = "\r\nPROCEEDING\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_PROCEEDING)), (UNITY_INT)((status)), (

   ((void *)0)

   ), (UNITY_UINT)(202), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_PROCEEDING_length(void)

{

    const char *input = "\r\nPROCEEDING\r\n";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((strlen(input))), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(212), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_WAIT_FOR_USER_DATA(void)

{

    const char *input = "> ";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((AT_CMD_WAIT_FOR_USER_DATA)), (UNITY_INT)((status)), (

   ((void *)0)

   ), (UNITY_UINT)(222), UNITY_DISPLAY_STYLE_INT);

}



void test_At_Parse_WAIT_FOR_USER_DATA_length(void)

{

    const char *input = "> ";

    AT_CommandStatus_t status = AT_CMD_INVALID;



    size_t n = AT_CommandStatusParse(input, strlen(input), &status);



    UnityAssertEqualNumber((UNITY_INT)((strlen(input))), (UNITY_INT)((n)), (

   ((void *)0)

   ), (UNITY_UINT)(232), UNITY_DISPLAY_STYLE_INT);

}
