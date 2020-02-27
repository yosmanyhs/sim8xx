#include "{{lowerCase name}}.h"
#include "test_{{lowerCase name}}_static.h"
#include "unity.h"

#include <string.h>
#include <stdio.h>

TEST_FILE("AtCommand.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_{{pascalCase name}}ObjectInit(void)
{
  {{pascalCase name}}_t {{lowerCase name}};
  {{pascalCase name}}ObjectInit(&{{lowerCase name}});

  TEST_ASSERT_EQUAL(&{{lowerCase name}}, {{lowerCase name}}.atcmd.obj);
  TEST_ASSERT_EQUAL_PTR({{pascalCase name}}Serialize, {{lowerCase name}}.atcmd.serialize);
  TEST_ASSERT_EQUAL_PTR({{pascalCase name}}Parse, {{lowerCase name}}.atcmd.parse);
  TEST_ASSERT_EQUAL_PTR({{pascalCase name}}Timeout, {{lowerCase name}}.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, {{lowerCase name}}.atcmd.timeoutInMilliSec);
}

void test_{{pascalCase name}}GetAtCommand(void)
{
  {{pascalCase name}}_t {{lowerCase name}};
  {{pascalCase name}}ObjectInit(&{{lowerCase name}});

  AT_Command_t *atcmd = {{pascalCase name}}GetAtCommand(&{{lowerCase name}});

  TEST_ASSERT_EQUAL(&{{lowerCase name}}, atcmd->obj);
  TEST_ASSERT_EQUAL_PTR({{pascalCase name}}Serialize, atcmd->serialize);
  TEST_ASSERT_EQUAL_PTR({{pascalCase name}}Parse, atcmd->parse);
  TEST_ASSERT_EQUAL_PTR({{pascalCase name}}Timeout, {{lowerCase name}}.atcmd.timeout);
  TEST_ASSERT_EQUAL(10000, atcmd->timeoutInMilliSec);
}

void test_{{pascalCase name}}Serialize(void)
{
  {{pascalCase name}}_t {{lowerCase name}};
  {{pascalCase name}}ObjectInit(&{{lowerCase name}});

  char obuf[32] = {0};
  size_t n      = {{pascalCase name}}Serialize(&{{lowerCase name}}, obuf, sizeof(obuf));

  const char *expected = "\r";
  size_t expectedLength = strlen(expected);

  TEST_ASSERT_EQUAL_STRING(expected, obuf);
  TEST_ASSERT_EQUAL(expectedLength, n);
}

void test_{{pascalCase name}}Parse_OK(void)
{
  {{pascalCase name}}_t {{lowerCase name}};
  {{pascalCase name}}ObjectInit(&{{lowerCase name}});

  const char *ibuf = "\r\nOK\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = {{pascalCase name}}Parse(&{{lowerCase name}}, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_OK, {{lowerCase name}}.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_{{pascalCase name}}Parse_ERROR(void)
{
  {{pascalCase name}}_t {{lowerCase name}};
  {{pascalCase name}}ObjectInit(&{{lowerCase name}});

  const char *ibuf = "\r\nERROR\r\n";
  size_t ilen    = strlen(ibuf);

  size_t n = {{pascalCase name}}Parse(&{{lowerCase name}}, ibuf, ilen);

  TEST_ASSERT_EQUAL(AT_CMD_ERROR, {{lowerCase name}}.response.status);
  TEST_ASSERT_EQUAL(ilen, n);
}

void test_{{pascalCase name}}Parse_Incomplete(void)
{
  {{pascalCase name}}_t {{lowerCase name}};
  {{pascalCase name}}ObjectInit(&{{lowerCase name}});

  const char *ibuf = "\r\nOK";
  size_t length    = strlen(ibuf);

  size_t n = {{pascalCase name}}Parse(&{{lowerCase name}}, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, {{lowerCase name}}.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_{{pascalCase name}}Parse_InvalidStatus(void)
{
  {{pascalCase name}}_t {{lowerCase name}};
  {{pascalCase name}}ObjectInit(&{{lowerCase name}});

  const char *ibuf = "\r\nPROCEEDING\r\n";
  size_t length    = strlen(ibuf);

  size_t n = {{pascalCase name}}Parse(&{{lowerCase name}}, ibuf, length);

  TEST_ASSERT_EQUAL(AT_CMD_INVALID, {{lowerCase name}}.response.status);
  TEST_ASSERT_EQUAL(0, n);
}

void test_{{pascalCase name}}Timeout(void)
{
  {{pascalCase name}}_t {{lowerCase name}};
  {{pascalCase name}}ObjectInit(&{{lowerCase name}});

  {{pascalCase name}}Timeout(&{{lowerCase name}});
  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, {{lowerCase name}}.response.status);
}
