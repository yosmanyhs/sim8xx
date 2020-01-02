#include "unity.h"

#include "Sim8xx.h"
#include "mock_Buffer.h"
#include "mock_Modem.h"
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

void test_SIM_Init(void)
{
  Sim8xxConfig_t config = {
    .put = serialPut,
  };

  GSM_ModemObjectInit_Expect(&sim8xx.modem);
  GSM_ModemRegisterPutFunction_ExpectAndReturn(&sim8xx.modem, serialPut, true);
  GSM_BufferObjectInit_Expect(&sim8xx.buffer);
  SIM_Init(&sim8xx, &config);
}

void cb(GSM_BluetoothEvent_t *p)
{
}

void test_SIM_BluetoothRegisterCallback(void)
{
  GSM_ModemRegisterBluetoothCallback_ExpectAndReturn(&sim8xx.modem, cb, true);
  SIM_RegisterBluetoothCallback(&sim8xx, cb);
}
