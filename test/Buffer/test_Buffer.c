#include "Buffer.h"
#include "unity.h"

#include <string.h>

#include "mock_Os.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_GSM_BufferObjectInit(void)
{
  Buffer_t buffer;
  GSM_BufferObjectInit(&buffer);

  TEST_ASSERT_EQUAL(0, buffer.rdindex);
  TEST_ASSERT_EQUAL(0, buffer.wrindex);
}

void test_GSM_BufferPutChar(void)
{
  Buffer_t buffer;
  GSM_BufferObjectInit(&buffer);

  size_t i;
  for (i = 0; i < 15; ++i) {
    char c = 'A' + i;
    OS_LockBuffer_Expect();
    OS_UnlockBuffer_Expect();
    bool result = GSM_BufferPutChar(&buffer, c);
    TEST_ASSERT(result);
  }

  for (i = 0; i < 15; ++i) {
    char c = 'A' + i;
    TEST_ASSERT_EQUAL(c, buffer.buffer[i]);
  }

  TEST_ASSERT_EQUAL(0, buffer.rdindex);
  TEST_ASSERT_EQUAL(15, buffer.wrindex);
}

void test_GSM_BufferPutChar_Overflow(void)
{
  Buffer_t buffer;
  GSM_BufferObjectInit(&buffer);

  size_t i;
  for (i = 0; i < SIM8XX_INPUT_BUFFER_LENGTH; ++i) {
    OS_LockBuffer_Expect();
    OS_UnlockBuffer_Expect();
    bool result = GSM_BufferPutChar(&buffer, 'A');
    TEST_ASSERT(result);
  }

  OS_LockBuffer_Expect();
  OS_UnlockBuffer_Expect();
  bool result = GSM_BufferPutChar(&buffer, 'B');
  TEST_ASSERT_FALSE(result);

  TEST_ASSERT_EQUAL(0, buffer.rdindex);
  TEST_ASSERT_EQUAL(SIM8XX_INPUT_BUFFER_LENGTH, buffer.wrindex);
}

void test_GSM_BufferGetData(void)
{
  Buffer_t buffer;
  GSM_BufferObjectInit(&buffer);

  size_t i;
  for (i = 0; i < 15; ++i) {
    char c = 'A' + i;
    OS_LockBuffer_Expect();
    OS_UnlockBuffer_Expect();
    GSM_BufferPutChar(&buffer, c);
  }

  OS_LockBuffer_Expect();
  OS_UnlockBuffer_Expect();
  Data_t idata = GSM_BufferGetData(&buffer);

  TEST_ASSERT_EQUAL(15, idata.length);

  for (i = 0; i < 15; ++i) {
    char c = 'A' + i;
    TEST_ASSERT_EQUAL(c, idata.data[i]);
  }

  TEST_ASSERT_EQUAL(0, buffer.rdindex);
  TEST_ASSERT_EQUAL(15, buffer.wrindex);
}


void test_GSM_BufferGetData_InternalError(void)
{
  Buffer_t buffer;
  GSM_BufferObjectInit(&buffer);

  buffer.rdindex = 16;
  buffer.wrindex = 15;

  OS_LockBuffer_Expect();
  OS_UnlockBuffer_Expect();
  Data_t idata = GSM_BufferGetData(&buffer);

  TEST_ASSERT_EQUAL_PTR(NULL, idata.data);
  TEST_ASSERT_EQUAL(0, idata.length);
  TEST_ASSERT_EQUAL(16, buffer.rdindex);
  TEST_ASSERT_EQUAL(15, buffer.wrindex);
}

void test_GSM_BufferClearData(void)
{
  Buffer_t buffer;
  GSM_BufferObjectInit(&buffer);

  size_t i;
  for (i = 0; i < 15; ++i) {
    char c = 'A' + i;
    OS_LockBuffer_Expect();
    OS_UnlockBuffer_Expect();
    GSM_BufferPutChar(&buffer, c);
  }

  TEST_ASSERT_EQUAL(0, buffer.rdindex);
  TEST_ASSERT_EQUAL(15, buffer.wrindex);

  OS_LockBuffer_Expect();
  OS_UnlockBuffer_Expect();
  bool result = GSM_BufferClearData(&buffer, 10);

  TEST_ASSERT(result);
  TEST_ASSERT_EQUAL(10, buffer.rdindex);
  TEST_ASSERT_EQUAL(15, buffer.wrindex);
}

void test_GSM_BufferClearData_InvalidLength(void)
{
  Buffer_t buffer;
  GSM_BufferObjectInit(&buffer);

  size_t i;
  for (i = 0; i < 15; ++i) {
    char c = 'A' + i;
    OS_LockBuffer_Expect();
    OS_UnlockBuffer_Expect();
    GSM_BufferPutChar(&buffer, c);
  }

  TEST_ASSERT_EQUAL(0, buffer.rdindex);
  TEST_ASSERT_EQUAL(15, buffer.wrindex);

  OS_LockBuffer_Expect();
  OS_UnlockBuffer_Expect();
  bool result = GSM_BufferClearData(&buffer, 16);

  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL(0, buffer.rdindex);
  TEST_ASSERT_EQUAL(15, buffer.wrindex);
}