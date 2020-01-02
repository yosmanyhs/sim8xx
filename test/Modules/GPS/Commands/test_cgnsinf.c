#include "cgnsinf.h"
#include "test_cgnsinf_static.h"
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

void test_CgnsInfObjectInit(void)
{
  CgnsInf_t cgnsinf;
  CgnsInfObjectInit(&cgnsinf);

  TEST_ASSERT_EQUAL(&cgnsinf, cgnsinf.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR(CgnsInfSerialize, cgnsinf.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR(CgnsInfParse, cgnsinf.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR(CgnsInfTimeout, cgnsinf.atcmd.timeout);
  TEST_ASSERT_EQUAL(10, cgnsinf.atcmd.timeoutInSec);
}

void test_CgnsInfGetAtCommand(void)
{
  CgnsInf_t cgnsinf;
  CgnsInfObjectInit(&cgnsinf);

  AT_Command_t *atcmd = CgnsInfGetAtCommand(&cgnsinf);

  TEST_ASSERT_EQUAL(&cgnsinf, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR(CgnsInfSerialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR(CgnsInfParse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR(CgnsInfTimeout, cgnsinf.atcmd.timeout);
  TEST_ASSERT_EQUAL(10, atcmd->timeoutInSec);
}

void test_CgnsInfSerialize(void)
{
  CgnsInf_t cgnsinf;
  CgnsInfObjectInit(&cgnsinf);

  char obuf[32] = {0};
  size_t n      = CgnsInfSerialize(&cgnsinf, obuf, sizeof(obuf));

  const char *expected = "AT+CGNSINF";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_CgnsInfParse(void)
{
  CgnsInf_t cgnsinf;
  CgnsInfObjectInit(&cgnsinf);

  const char *ibuf = "\r\n+CGNSINF: 1,1,20180925163521.000,31.221303,121.355042,71.900,0.00,45.1,1,,1.0,1.3,0.8,,10,10,6,,36,0,8\r\n\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = CgnsInfParse(&cgnsinf, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, cgnsinf.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(1, cgnsinf.response.runStatus);
  TEST_ASSERT_EQUAL(1, cgnsinf.response.fixStatus);
  TEST_ASSERT_EQUAL_STRING("20180925163521.000", cgnsinf.response.date);
  TEST_ASSERT_DOUBLE_WITHIN(0.1, 31.221303, cgnsinf.response.latitude);
  TEST_ASSERT_DOUBLE_WITHIN(0.1, 121.355042, cgnsinf.response.longitude);
  TEST_ASSERT_DOUBLE_WITHIN(0.1, 71.900, cgnsinf.response.altitude);
  TEST_ASSERT_EQUAL_DOUBLE(0.0, cgnsinf.response.speed);
  TEST_ASSERT_DOUBLE_WITHIN(0.1, 45.1, cgnsinf.response.course);
  TEST_ASSERT_EQUAL(1, cgnsinf.response.fixMode);
  TEST_ASSERT_DOUBLE_WITHIN(0.1, 1.0, cgnsinf.response.hdop);
  TEST_ASSERT_DOUBLE_WITHIN(0.1, 1.3, cgnsinf.response.pdop);
  TEST_ASSERT_DOUBLE_WITHIN(0.1, 0.8, cgnsinf.response.vdop);
  TEST_ASSERT_EQUAL(10, cgnsinf.response.gpsSatInView);
  TEST_ASSERT_EQUAL(10, cgnsinf.response.gnssSatInUse);
  TEST_ASSERT_EQUAL(6, cgnsinf.response.gnssSatInView);
  TEST_ASSERT_EQUAL(36, cgnsinf.response.cnomax);
  TEST_ASSERT_EQUAL(0, cgnsinf.response.hpa);
  TEST_ASSERT_EQUAL(8, cgnsinf.response.vpa);
}

void test_CgnsInfParse_Incomplete(void)
{
  CgnsInf_t cgnsinf;
  CgnsInfObjectInit(&cgnsinf);

  const char *ibuf = "\r\n+CGNSINF: 1,1,20180925163521.000,31.2213";
  size_t length    = strlen(ibuf);

  size_t n = CgnsInfParse(&cgnsinf, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cgnsinf.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CgnsInfParse_InvalidStatus(void)
{
  CgnsInf_t cgnsinf;
  CgnsInfObjectInit(&cgnsinf);

  const char *ibuf = "\r\nERROR\r\n";
  size_t length    = strlen(ibuf);

  size_t n = CgnsInfParse(&cgnsinf, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, cgnsinf.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_CgnsInfTimeout(void)
{
  CgnsInf_t cgnsinf;
  CgnsInfObjectInit(&cgnsinf);

  CgnsInfTimeout(&cgnsinf);

  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, cgnsinf.response.status);
}
