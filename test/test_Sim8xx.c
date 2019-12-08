#include "unity.h"
#include "test_Sim8xx_static.h"

#include <string.h>
#include <stdio.h>

#include "mock_Modem.h"
#include "Sim8xx.h"


void setUp(void)
{
}

void tearDown(void)
{
}

void test_SIM_Init(void)
{
  GSM_ModemObjectInit_Expect(&sim8xx);
  SIM_Init();
}

void bluetoothCallback(GSM_BluetoothEvent_t *p)
{

}

void test_SIM_BluetoothRegisterCallback(void)
{
  GSM_ModemRegisterBluetoothCallback_ExpectAndReturn(&sim8xx, bluetoothCallback, true);
  SIM_BluetoothRegisterCallback(bluetoothCallback);
}

void test_SIM_ProcessInput(void)
{
  char ibuf[1];
  char ilen = 1;
  GSM_ModemParse_ExpectAndReturn(&sim8xx, ibuf, ilen, 1);
  SIM_ProcessInput(ibuf, ilen);
}