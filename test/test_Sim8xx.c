#include "unity.h"
#include "Sim8xx.h"
#include "mock_Buffer.h"
#include "mock_Modem.h"
#include "mock_Os.h"
#include "test_Sim8xx_static.h"

#include <stdio.h>
#include <string.h>

static Sim8xx_t sim8xx;

void setUp(void)
{
}

void tearDown(void)
{
}

void serialPut(char c)
{
}

static Sim8xxConfig_t config = {
    .put = serialPut,
};

void test_SIM_Init(void)
{
  GSM_ModemObjectInit_Expect(&sim8xx.modem);
  GSM_ModemRegisterPutFunction_ExpectAndReturn(&sim8xx.modem, serialPut, true);
  GSM_BufferObjectInit_Expect(&sim8xx.buffer);
  OS_Init_Expect();
  SIM_Init(&sim8xx, &config);
}

void test_SIM_Start(void)
{
  GSM_ModemObjectInit_Expect(&sim8xx.modem);
  GSM_ModemRegisterPutFunction_ExpectAndReturn(&sim8xx.modem, serialPut, true);
  GSM_BufferObjectInit_Expect(&sim8xx.buffer);
  OS_Init_Expect();
  SIM_Init(&sim8xx, &config);

  GSM_ModemIsAlive_ExpectAndReturn(&sim8xx.modem, true);
  GSM_ModemDisableEcho_ExpectAndReturn(&sim8xx.modem, true);
  bool result = SIM_Start(&sim8xx);

  TEST_ASSERT(result);
}

void test_SIM_Start_NotAlive(void)
{
  GSM_ModemObjectInit_Expect(&sim8xx.modem);
  GSM_ModemRegisterPutFunction_ExpectAndReturn(&sim8xx.modem, serialPut, true);
  GSM_BufferObjectInit_Expect(&sim8xx.buffer);
  OS_Init_Expect();
  SIM_Init(&sim8xx, &config);

  GSM_ModemIsAlive_ExpectAndReturn(&sim8xx.modem, false);
  bool result = SIM_Start(&sim8xx);

  TEST_ASSERT_FALSE(result);
}

void test_SIM_Start_DisableEchoFails(void)
{
  GSM_ModemObjectInit_Expect(&sim8xx.modem);
  GSM_ModemRegisterPutFunction_ExpectAndReturn(&sim8xx.modem, serialPut, true);
  GSM_BufferObjectInit_Expect(&sim8xx.buffer);
  OS_Init_Expect();
  SIM_Init(&sim8xx, &config);

  GSM_ModemIsAlive_ExpectAndReturn(&sim8xx.modem, true);
  GSM_ModemDisableEcho_ExpectAndReturn(&sim8xx.modem, false);
  bool result = SIM_Start(&sim8xx);

  TEST_ASSERT_FALSE(result);
}

void cb(GSM_BluetoothEvent_t *p)
{
}

void test_SIM_BluetoothRegisterCallback(void)
{
  GSM_ModemRegisterBluetoothCallback_ExpectAndReturn(&sim8xx.modem, cb, true);
  SIM_RegisterBluetoothCallback(&sim8xx, cb);
}

void test_SIM_Parse(void)
{
  GSM_ModemObjectInit_Expect(&sim8xx.modem);
  GSM_ModemRegisterPutFunction_ExpectAndReturn(&sim8xx.modem, serialPut, true);
  GSM_BufferObjectInit_Expect(&sim8xx.buffer);
  OS_Init_Expect();
  SIM_Init(&sim8xx, &config);

  GSM_BufferData_t ibuf = {
    .data = NULL,
    .length = 0,
  };

  // First round: 6 chars parsed, 4 left in the buffer.
  GSM_BufferGetData_ExpectAndReturn(&sim8xx.buffer, ibuf);
  GSM_ModemParse_ExpectAndReturn(&sim8xx.modem, ibuf.data, ibuf.length, 6);
  GSM_BufferPopData_ExpectAndReturn(&sim8xx.buffer, 6, true);
  GSM_BufferGetLength_ExpectAndReturn(&sim8xx.buffer, 4);

  // Second round: 0 chars parsed, 4 left in the buffer and exit.
  GSM_BufferGetData_ExpectAndReturn(&sim8xx.buffer, ibuf);
  GSM_ModemParse_ExpectAndReturn(&sim8xx.modem, ibuf.data, ibuf.length, 0);

  SIM_Parse(&sim8xx);
}
