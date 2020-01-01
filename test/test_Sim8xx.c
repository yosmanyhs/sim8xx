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

void test_SIM_Init(void)
{
  GSM_ModemObjectInit_Expect(&sim8xx.modem);
  GSM_BufferObjectInit_Expect(&sim8xx.buffer);
  SIM_Init(&sim8xx);
}

void cb(GSM_BluetoothEvent_t *p)
{
}

void test_SIM_BluetoothRegisterCallback(void)
{
  GSM_ModemRegisterBluetoothCallback_ExpectAndReturn(&sim8xx.modem, cb, true);
  SIM_BluetoothRegisterCallback(&sim8xx, cb);
}
