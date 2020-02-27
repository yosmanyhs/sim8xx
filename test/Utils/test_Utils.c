#include "Utils.h"
#include "unity.h"

#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_GSM_UtilsGetString(void)
{
    const char *ibuf = "asd\"text to parse\"extra characters .";
    size_t ilen = strlen(ibuf);

    char obuf[20] = {0};
    size_t olen = sizeof(obuf);

    size_t n = GSM_UtilsGetString(ibuf, ilen, obuf, olen, '"', '"');

    TEST_ASSERT_EQUAL(17, n);
    TEST_ASSERT_EQUAL_STRING("text to parse", obuf);
}

void test_GSM_UtilsGetString_NoString(void)
{
    const char *ibuf = "asd text to parse extra characters .";
    size_t ilen = strlen(ibuf);

    char obuf[20] = {0};
    size_t olen = sizeof(obuf);

    size_t n = GSM_UtilsGetString(ibuf, ilen, obuf, olen, '"', '"');

    TEST_ASSERT_EQUAL(0, n);
    TEST_ASSERT_EQUAL_STRING("", obuf);
}

void test_GSM_UtilsGetString_NoEnding(void)
{
    const char *ibuf = "asd\"text to parse extra characters .";
    size_t ilen = strlen(ibuf);

    char obuf[20] = {0};
    size_t olen = sizeof(obuf);

    size_t n = GSM_UtilsGetString(ibuf, ilen, obuf, olen, '"', '"');

    TEST_ASSERT_EQUAL(0, n);
    TEST_ASSERT_EQUAL_STRING("", obuf);
}

void test_GSM_UtilsGetInt(void)
{
    const char *ibuf = "asd\"int to parse: ,192, extra characters .";
    size_t ilen = strlen(ibuf);

    int32_t d = 0;

    size_t n = GSM_UtilsGetInt(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL(192, d);
    TEST_ASSERT_EQUAL(22, n);
}

void test_GSM_UtilsGetInt_NoInt(void)
{
    const char *ibuf = "asd\"int to parse: 192 extra characters .";
    size_t ilen = strlen(ibuf);

    int32_t d = 0;

    size_t n = GSM_UtilsGetInt(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL(0, d);
    TEST_ASSERT_EQUAL(0, n);
}

void test_GSM_UtilsGetInt_ExtraCharacters(void)
{
    const char *ibuf = "asd\"int to parse: ,192sss, extra characters .";
    size_t ilen = strlen(ibuf);

    int32_t d = 0;

    size_t n = GSM_UtilsGetInt(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL(192, d);
    TEST_ASSERT_EQUAL(25, n);
}

void test_GSM_UtilsGetInt_NegativeInt(void)
{
    const char *ibuf = "asd\"int to parse: ,-299, extra characters .";
    size_t ilen = strlen(ibuf);

    int32_t d = 0;

    size_t n = GSM_UtilsGetInt(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL(-299, d);
    TEST_ASSERT_EQUAL(23, n);
}

void test_GSM_UtilsGetDouble(void)
{
    const char *ibuf = "asd\"int to parse: ,3.14159265, extra characters .";
    size_t ilen = strlen(ibuf);

    double d = 0;

    size_t n = GSM_UtilsGetDouble(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL(3.14159265, d);
    TEST_ASSERT_EQUAL(29, n);
}

void test_GSM_UtilsGetDouble_NoDouble(void)
{
    const char *ibuf = "asd\"int to parse:  extra characters .";
    size_t ilen = strlen(ibuf);

    double d = 0;

    size_t n = GSM_UtilsGetDouble(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL_DOUBLE(0, d);
    TEST_ASSERT_EQUAL(0, n);
}

void test_GSM_UtilsGetDouble_Negative(void)
{
    const char *ibuf = "asd\"int to parse: ,-3.14159265, extra characters .";
    size_t ilen = strlen(ibuf);

    double d = 0;

    size_t n = GSM_UtilsGetDouble(ibuf, ilen, &d, ',', ',');

    TEST_ASSERT_EQUAL_DOUBLE(-3.14159265, d);
    TEST_ASSERT_EQUAL(30, n);
}

void test_GSM_UtilsSkipReserved_7(void)
{
    const char *ibuf = "01,11,21,31,41,51,61,71,81,91,101.";
    size_t ilen = strlen(ibuf);

    size_t n = GSM_UtilsSkipReserved(ibuf, ilen, ',', 7);

    TEST_ASSERT_EQUAL(23,n);
}

void test_GSM_UtilsSkipReserved_1(void)
{
    const char *ibuf = "01,11,21,31,41,51,61,71,81,91,101.";
    size_t ilen = strlen(ibuf);

    size_t n = GSM_UtilsSkipReserved(ibuf, ilen, ',', 1);

    TEST_ASSERT_EQUAL(5,n);
}

void test_GSM_UtilsSkipReserved_2(void)
{
    const char *ibuf = "01,11,21,31,41,51,61,71,81,91,101.";
    size_t ilen = strlen(ibuf);

    size_t n = GSM_UtilsSkipReserved(ibuf, ilen, ',', 2);

    TEST_ASSERT_EQUAL(8,n);
}

void test_GSM_UtilsSkipReserved_NotFound(void)
{
    const char *ibuf = "01,11,21,31,41,51,61,71,81,91,101.";
    size_t ilen = strlen(ibuf);

    size_t n = GSM_UtilsSkipReserved(ibuf, ilen, ':', 1);

    TEST_ASSERT_EQUAL(0,n);
}

void test_GSM_UtilsSkipReserved_FoundLess(void)
{
    const char *ibuf = "01,11,";
    size_t ilen = strlen(ibuf);

    size_t n = GSM_UtilsSkipReserved(ibuf, ilen, ',', 2);

    TEST_ASSERT_EQUAL(0,n);
}

void test_GSM_UtilsItoA_Negative(void)
{
    char obuf[20] = {0};
    size_t olen = sizeof(obuf);

    size_t n = GSM_UtilsItoA(obuf, olen, -39);

    TEST_ASSERT_EQUAL(3, n);
    TEST_ASSERT_EQUAL_STRING("-39", obuf);
}

void test_GSM_UtilsItoA_Positive(void)
{
    char obuf[20] = {0};
    size_t olen = sizeof(obuf);

    size_t n = GSM_UtilsItoA(obuf, olen, 4096);

    TEST_ASSERT_EQUAL(4, n);
    TEST_ASSERT_EQUAL_STRING("4096", obuf);
}

void test_GSM_UtilsItoA_Zero(void)
{
    char obuf[20] = {0};
    size_t olen = sizeof(obuf);

    size_t n = GSM_UtilsItoA(obuf, olen, 0);

    TEST_ASSERT_EQUAL(1, n);
    TEST_ASSERT_EQUAL_STRING("0", obuf);
}

void test_GSM_UtilsItoA_BufferTooSmall(void)
{
    char obuf[4] = {0};
    size_t olen = sizeof(obuf);

    size_t n = GSM_UtilsItoA(obuf, olen, 4096);

    TEST_ASSERT_EQUAL(0, n);
    TEST_ASSERT_EQUAL_STRING("", obuf);
}

void test_GSM_UtilsMatch_Match(void)
{
    const char *stra = "strings do match";
    const char *strb = "strings do match";

    TEST_ASSERT(GSM_UtilsMatch(stra, strb, strlen(stra)));
}

void test_GSM_UtilsMatch_NoMatch_1(void)
{
    const char *stra = "strings do match";
    const char *strb = "strings don't match";

    TEST_ASSERT_FALSE(GSM_UtilsMatch(stra, strb, strlen(stra)));
}

void test_GSM_UtilsMatch_NoMatch_2(void)
{
    const char *stra = "strings don't match";
    const char *strb = "strings do match";

    TEST_ASSERT_FALSE(GSM_UtilsMatch(stra, strb, strlen(stra)));
}

void test_GSM_UtilsBeginsWith_TooShort(void)
{
    const char *str = "Too short";
    const char *pre = "Too short string";

    TEST_ASSERT_FALSE(GSM_UtilsBeginsWith(str, pre));
}

void test_GSM_UtilsBeginsWith_Match(void)
{
    const char *str = "Long enough string";
    const char *pre = "Long enough";

    TEST_ASSERT(GSM_UtilsBeginsWith(str, pre));
}

void test_GSM_UtilsBeginsWith_NoMatch(void)
{
    const char *str = "Long enough string";
    const char *pre = "No match";

    TEST_ASSERT_FALSE(GSM_UtilsBeginsWith(str, pre));
}

void test_GSM_UtilsBeginsWith_MatchWithDifferentCase(void)
{
    const char *str = "Long Enough string";
    const char *pre = "long enough";

    TEST_ASSERT(GSM_UtilsBeginsWith(str, pre));
}

void test_GSM_UtilsEndsWith_TooShort(void)
{
    const char *str = "Too short";
    const char *post = "Too short string";

    TEST_ASSERT_FALSE(GSM_UtilsEndsWith(str, post));
}

void test_GSM_UtilsEndsWith_Match(void)
{
    const char *str = "Long enough string";
    const char *post = "enough string";

    TEST_ASSERT(GSM_UtilsEndsWith(str, post));
}

void test_GSM_UtilsEndsWith_NoMatch(void)
{
    const char *str = "Long enough string";
    const char *post = "different string";

    TEST_ASSERT_FALSE(GSM_UtilsEndsWith(str, post));
}

void test_GSM_UtilsEndsWith_MatchWithDifferentCase(void)
{
    const char *str = "Long enough string";
    const char *post = "ENOUGH STRING";

    TEST_ASSERT(GSM_UtilsEndsWith(str, post));
}