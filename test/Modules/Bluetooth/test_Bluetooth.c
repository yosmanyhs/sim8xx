#include "Bluetooth.h"
#include "unity.h"
#include <string.h>

#include "mock_test_Bluetooth_callback.h"

TEST_FILE("btconnect.c");
TEST_FILE("btpower.c");
TEST_FILE("bthost.c");
TEST_FILE("btsppsend.c");
TEST_FILE("btpaircfg.c");
TEST_FILE("Modem.c");
TEST_FILE("At.c");
TEST_FILE("Utils.c");

void setUp(void)
{
}

void tearDown(void)
{
}

void test_Bluetooth_URC_BTCONNECTING(void)
{
  const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"SPP\"\r\n";
  size_t ilen = strlen(ibuf);

  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  BTcallback_Expect(&bluetooth.event);
  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(GSM_BT_BTCONNECTING, bluetooth.event.id);  
  TEST_ASSERT_EQUAL_STRING("34:c7:31:aa:37:5b", bluetooth.event.payload.btconnecting.address);
  TEST_ASSERT_EQUAL_STRING("SPP", bluetooth.event.payload.btconnecting.profile);
}

void test_Bluetooth_URC_BTCONNECTING_Incomplete(void)
{
  const char *ibuf = "\r\n+BTCONNECTING: \"34:c7:31:aa:37:5b\",\"SP";
  size_t ilen = strlen(ibuf);

  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(0, n);
  TEST_ASSERT_EQUAL(GSM_BT_NO_EVENT, bluetooth.event.id);  
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.btconnecting.address);
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.btconnecting.profile);
}

void test_Bluetooth_URC_BTCONNECT(void)
{
  const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:10,\"HFP(AG)\"\r\n";
  size_t ilen = strlen(ibuf);

  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  BTcallback_Expect(&bluetooth.event);
  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(GSM_BT_BTCONNECT, bluetooth.event.id);  
  TEST_ASSERT_EQUAL(1, bluetooth.event.payload.btconnect.id);
  TEST_ASSERT_EQUAL_STRING("MK-ZHANZHIMIN", bluetooth.event.payload.btconnect.name);
  TEST_ASSERT_EQUAL_STRING("00:1a:7d:da:71:10", bluetooth.event.payload.btconnect.address);
  TEST_ASSERT_EQUAL_STRING("HFP(AG)", bluetooth.event.payload.btconnect.profile);
}

void test_Bluetooth_URC_BTCONNECT_Incomplete(void)
{
  const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:7";
  size_t ilen = strlen(ibuf);

  GSM_Bluetooth_t bluetooth;
  GSM_BluetoothObjectInit(&bluetooth);
  GSM_BluetoothRegisterCallback(&bluetooth, BTcallback);

  size_t n = GSM_BluetoothURCParse(&bluetooth, ibuf, ilen);

  TEST_ASSERT_EQUAL(0, n);
  TEST_ASSERT_EQUAL(GSM_BT_NO_EVENT, bluetooth.event.id);  
  TEST_ASSERT_EQUAL(0, bluetooth.event.payload.btconnect.id);
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.btconnect.name);
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.btconnect.address);
  TEST_ASSERT_EQUAL_STRING("", bluetooth.event.payload.btconnect.profile);
}