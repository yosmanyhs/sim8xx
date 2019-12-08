#include "Bluetooth.h"
#include "unity.h"
#include "mock_Modem.h"
#include <string.h>

#include "mock_test_Bluetooth_callback.h"

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

void setUp(void)
{
}

void tearDown(void)
{
}

void test_GSM_BluetoothObjectInit(void)
{
  GSM_Modem_t modem;
  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth, &modem);

  TEST_ASSERT_EQUAL_PTR(&modem, bluetooth.parent);
  TEST_ASSERT_EQUAL_PTR(NULL, bluetooth.notify);
}

void bluetoothCallback(GSM_BluetoothEvent_t *p)
{

}

void test_GSM_BluetoothRegisterCallback(void)
{
  GSM_Modem_t modem;
  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth, &modem);
  bool result = GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);
  
  TEST_ASSERT(result);
  TEST_ASSERT_EQUAL_PTR(BTcallback, bluetooth.notify);
}

void test_GSM_BluetoothRegisterCallback_SecondAttemptFails(void)
{
  GSM_Modem_t modem;
  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth, &modem);
  bool result = GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);
  TEST_ASSERT_EQUAL_PTR(BTcallback, bluetooth.notify);
  TEST_ASSERT(result);

  result = GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);
  TEST_ASSERT_FALSE(result);
}

void test_GSM_BluetoothURCParse_URC_BTCONNECTING(void)
{
  const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"SPP\"\r\n";
  size_t ilen = strlen(ibuf);

  GSM_Modem_t modem;
  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth, &modem);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  BTcallback_Expect(&bluetooth.event);
  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(GSM_BT_CONNECTING, bluetooth.event.type);  
  TEST_ASSERT_EQUAL_STRING("34:c7:31:aa:37:5b", bluetooth.event.payload.connecting.address);
  TEST_ASSERT_EQUAL_STRING("SPP", bluetooth.event.payload.connecting.profile);
}

void test_GSM_BluetoothURCParse_URC_BTCONNECTING_Incomplete(void)
{
  const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"SP";
  size_t ilen = strlen(ibuf);

  GSM_Modem_t modem;
  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth, &modem);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(0, n);
  TEST_ASSERT_EQUAL(GSM_BT_NO_EVENT, bluetooth.event.type);  
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.connecting.address);
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.connecting.profile);
}

void test_GSM_BluetoothURCParse_URC_BTCONNECT(void)
{
  const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:10,\"HFP(AG)\"\r\n";
  size_t ilen = strlen(ibuf);

  GSM_Modem_t modem;
  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth, &modem);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  BTcallback_Expect(&bluetooth.event);
  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(GSM_BT_CONNECTED, bluetooth.event.type);  
  TEST_ASSERT_EQUAL(1, bluetooth.event.payload.connected.id);
  TEST_ASSERT_EQUAL_STRING("MK-ZHANZHIMIN", bluetooth.event.payload.connected.name);
  TEST_ASSERT_EQUAL_STRING("00:1a:7d:da:71:10", bluetooth.event.payload.connected.address);
  TEST_ASSERT_EQUAL_STRING("HFP(AG)", bluetooth.event.payload.connected.profile);
}

void test_GSM_BluetoothURCParse_URC_BTCONNECT_Incomplete(void)
{
  const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:7";
  size_t ilen = strlen(ibuf);

  GSM_Modem_t modem;
  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth, &modem);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(0, n);
  TEST_ASSERT_EQUAL(GSM_BT_NO_EVENT, bluetooth.event.type);  
  TEST_ASSERT_EQUAL(0, bluetooth.event.payload.connected.id);
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.connected.name);
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.connected.address);
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.connected.profile);
}

void test_GSM_BluetoothURCParse_URC_SPPGET(void)
{
  const char *ibuf = "\r\n+BTSPPDATA: 1,15,SIMCOMSPPFORAPP\r\n";
  size_t ilen = strlen(ibuf);

  GSM_Modem_t modem;
  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth, &modem);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  BTcallback_Expect(&bluetooth.event);
  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(GSM_BT_INCOMING_DATA, bluetooth.event.type);  
  TEST_ASSERT_EQUAL_STRING("SIMCOMSPPFORAPP", bluetooth.event.payload.incomingData.data);
}

void test_GSM_BluetoothURCParse_URC_SPPGET_Incomplete(void)
{
  const char *ibuf = "\r\n+BTSPPDATA: 1,15,SIMCOMSPPFORAPP";
  size_t ilen = strlen(ibuf);

  GSM_Modem_t modem;
  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth, &modem);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(0, n);
  TEST_ASSERT_EQUAL(GSM_BT_NO_EVENT, bluetooth.event.type);  
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.incomingData.data);
}

void test_GSM_BluetoothURCParse_URC_DISCONN(void)
{
  const char *ibuf = "\r\n+BTDISCONN:\"SIM800H\",34:c7:31:aa:37:5b,\"SPP\"\r\n";
  size_t ilen = strlen(ibuf);

  GSM_Modem_t modem;
  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth, &modem);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  BTcallback_Expect(&bluetooth.event);
  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(GSM_BT_DISCONNECTED, bluetooth.event.type);  
  TEST_ASSERT_EQUAL_STRING("SIM800H", bluetooth.event.payload.disconnected.name);
}

void test_GSM_BluetoothURCParse_URC_DISCONN_Incomplete(void)
{
  const char *ibuf = "\r\n+BTDISCONN:\"SIM800H\",34:c7:31:aa:";
  size_t ilen = strlen(ibuf);

  GSM_Modem_t modem;
  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth, &modem);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(0, n);
  TEST_ASSERT_EQUAL(GSM_BT_NO_EVENT, bluetooth.event.type);  
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.disconnected.name);
}