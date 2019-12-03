#include "Utils.h"
#include "unity.h"

#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_Utils_GetString(void)
{
    const char *ibuf = "asd\"text to parse\"extra characters .";
    size_t ilen = strlen(ibuf);

    char obuf[20] = {0};
    size_t olen = sizeof(obuf);

    size_t n = GSM_UtilsGetString(ibuf, ilen, obuf, olen, '"');

    TEST_ASSERT_EQUAL(17, n);
    TEST_ASSERT_EQUAL_STRING("text to parse", obuf);
}

void test_Utils_GetString_NoString(void)
{
    const char *ibuf = "asd text to parse extra characters .";
    size_t ilen = strlen(ibuf);

    char obuf[20] = {0};
    size_t olen = sizeof(obuf);

    size_t n = GSM_UtilsGetString(ibuf, ilen, obuf, sizeof(obuf), '"');

    TEST_ASSERT_EQUAL(0, n);
    TEST_ASSERT_EQUAL_STRING("", obuf);
}

void test_Utils_GetString_NoEnding(void)
{
    const char *ibuf = "asd\"text to parse extra characters .";
    size_t ilen = strlen(ibuf);

    char obuf[20] = {0};
    size_t olen = sizeof(obuf);

    size_t n = GSM_UtilsGetString(ibuf, ilen, obuf, sizeof(obuf), '"');

    TEST_ASSERT_EQUAL(0, n);
    TEST_ASSERT_EQUAL_STRING("", obuf);
}

void test_Utils_GetInt(void)
{
    const char *ibuf = "asd\"int to parse: ,192, extra characters .";
    size_t ilen = strlen(ibuf);

    int d = 0;

    size_t n = GSM_UtilsGetInt(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL(192, d);
    TEST_ASSERT_EQUAL(22, n);
}

void test_Utils_GetInt_NoInt(void)
{
    const char *ibuf = "asd\"int to parse: 192 extra characters .";
    size_t ilen = strlen(ibuf);

    int d = 0;

    size_t n = GSM_UtilsGetInt(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL(0, d);
    TEST_ASSERT_EQUAL(0, n);
}

void test_Utils_GetInt_ExtraCharacters(void)
{
    const char *ibuf = "asd\"int to parse: ,192sss, extra characters .";
    size_t ilen = strlen(ibuf);

    int d = 0;

    size_t n = GSM_UtilsGetInt(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL(192, d);
    TEST_ASSERT_EQUAL(25, n);
}

void test_Utils_GetInt_NegativeInt(void)
{
    const char *ibuf = "asd\"int to parse: ,-299, extra characters .";
    size_t ilen = strlen(ibuf);

    int d = 0;

    size_t n = GSM_UtilsGetInt(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL(-299, d);
    TEST_ASSERT_EQUAL(23, n);
}

void test_Utils_GetDouble(void)
{
    const char *ibuf = "asd\"int to parse: ,3.14159265, extra characters .";
    size_t ilen = strlen(ibuf);

    double d = 0;

    size_t n = GSM_UtilsGetDouble(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL(3.14159265, d);
    TEST_ASSERT_EQUAL(29, n);
}

void test_Utils_GetDouble_NoDouble(void)
{
    const char *ibuf = "asd\"int to parse:  extra characters .";
    size_t ilen = strlen(ibuf);

    double d = 0;

    size_t n = GSM_UtilsGetDouble(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL_DOUBLE(0, d);
    TEST_ASSERT_EQUAL(0, n);
}

void test_Utils_GetDouble_Negative(void)
{
    const char *ibuf = "asd\"int to parse: ,-3.14159265, extra characters .";
    size_t ilen = strlen(ibuf);

    double d = 0;

    size_t n = GSM_UtilsGetDouble(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL_DOUBLE(-3.14159265, d);
    TEST_ASSERT_EQUAL(30, n);
}

void test_Utils_SkipReserved_7(void)
{
    const char *ibuf = "01,11,21,31,41,51,61,71,81,91,101.";
    size_t ilen = strlen(ibuf);

    size_t n = GSM_UtilsSkipReserved(ibuf, ilen, ',', 7);

    TEST_ASSERT_EQUAL(23,n);
}

void test_Utils_SkipReserved_1(void)
{
    const char *ibuf = "01,11,21,31,41,51,61,71,81,91,101.";
    size_t ilen = strlen(ibuf);

    size_t n = GSM_UtilsSkipReserved(ibuf, ilen, ',', 1);

    TEST_ASSERT_EQUAL(5,n);
}

void test_Utils_SkipReserved_2(void)
{
    const char *ibuf = "01,11,21,31,41,51,61,71,81,91,101.";
    size_t ilen = strlen(ibuf);

    size_t n = GSM_UtilsSkipReserved(ibuf, ilen, ',', 2);

    TEST_ASSERT_EQUAL(8,n);
}

void test_Utils_SkipReserved_NotFound(void)
{
    const char *ibuf = "01,11,21,31,41,51,61,71,81,91,101.";
    size_t ilen = strlen(ibuf);

    size_t n = GSM_UtilsSkipReserved(ibuf, ilen, ':', 1);

    TEST_ASSERT_EQUAL(0,n);
}

void test_Utils_SkipReserved_FoundLess(void)
{
    const char *ibuf = "01,11,";
    size_t ilen = strlen(ibuf);

    size_t n = GSM_UtilsSkipReserved(ibuf, ilen, ',', 2);

    TEST_ASSERT_EQUAL(0,n);
}