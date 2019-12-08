#include "unity.h"

#include "Modem.h"
#include "mock_Os.h"
#include "mock_Serial.h"

TEST_FILE("btacpt.c");
TEST_FILE("btconnect.c");
TEST_FILE("btdisconn.c");
TEST_FILE("btpower.c");
TEST_FILE("bthost.c");
TEST_FILE("btsppget.c");
TEST_FILE("btsppsend.c");
TEST_FILE("btpaircfg.c");
TEST_FILE("At.c");
TEST_FILE("Utils.c");
TEST_FILE("Bluetooth.c")

void setUp(void)
{
}

void tearDown(void)
{
}

void test_GSM_ModemObjectInit(void)
{
  GSM_Modem_t modem;
  GSM_ModemObjectInit(&modem);

  TEST_ASSERT_EQUAL_PTR(NULL, modem.currentAt);
  TEST_ASSERT_EQUAL_PTR(&modem, modem.bluetooth.parent);
}

void bluetoothCallback(GSM_BluetoothEvent_t *p)
{

}

void test_GSM_ModemRegisterBluetoothCallback(void)
{
  GSM_Modem_t modem;
  GSM_ModemObjectInit(&modem);
  GSM_ModemRegisterBluetoothCallback(&modem, bluetoothCallback);

  TEST_ASSERT_EQUAL_PTR(bluetoothCallback, modem.bluetooth.notify);
}

void test_GSM_ModemParse(void)
{
  GSM_Modem_t modem;
  GSM_ModemObjectInit(&modem);
  GSM_ModemRegisterBluetoothCallback(&modem, bluetoothCallback);

  char ibuf[] = "";
  size_t ilen = strlen(ibuf);
}