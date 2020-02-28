#include "unity.h"

#include "Modem.h"
#include "mock_Os.h"
#include "mock_test_Bluetooth_callback.h"

#include <string.h>

#include "btconnect.h"
#include "btpower.h"

TEST_FILE("at.c")
TEST_FILE("atcfun.c")
TEST_FILE("atcpin.c")
TEST_FILE("ate.c")
TEST_FILE("btacpt.c");
TEST_FILE("btconnect.c");
TEST_FILE("btdisconn.c");
TEST_FILE("btpower.c");
TEST_FILE("bthost.c");
TEST_FILE("btsppget.c");
TEST_FILE("btsppsend.c");
TEST_FILE("btpair.c");
TEST_FILE("btpaircfg.c");
TEST_FILE("cgnspwr.c");
TEST_FILE("cgnsinf.c");
TEST_FILE("AtCommand.c");
TEST_FILE("Utils.c");
TEST_FILE("Bluetooth.c")
TEST_FILE("Gps.c")
TEST_FILE("Ip.c")
TEST_FILE("sapbr.c")
TEST_FILE("httpinit.c")
TEST_FILE("httppara.c")
TEST_FILE("httpaction.c")
TEST_FILE("httpterm.c")

void setUp(void)
{
}

void tearDown(void)
{
}

void put(char c)
{
}

void test_GSM_ModemObjectInit(void)
{
  GSM_Modem_t modem;
  GSM_ModemObjectInit(&modem);

  TEST_ASSERT_EQUAL_PTR(NULL, modem.currentAt);
  TEST_ASSERT_EQUAL_PTR(NULL, modem.put);
  TEST_ASSERT_EQUAL_PTR(&modem, modem.bluetooth.parent);
  TEST_ASSERT_EQUAL_PTR(&modem, modem.gps.parent);
}

void test_GSM_ModemRegisterPutFunction(void)
{
  GSM_Modem_t modem;
  GSM_ModemObjectInit(&modem);
  GSM_ModemRegisterPutFunction(&modem, put);

  TEST_ASSERT_EQUAL_PTR(put, modem.put);
}

void test_GSM_ModemRegisterBluetoothCallback(void)
{
  GSM_Modem_t modem;
  GSM_ModemObjectInit(&modem);
  GSM_ModemRegisterBluetoothCallback(&modem, BTcallback);

  TEST_ASSERT_EQUAL_PTR(BTcallback, modem.bluetooth.notify);
}

void test_GSM_ModemExecuteAtCommand(void)
{
  GSM_Modem_t modem;
  GSM_ModemObjectInit(&modem);
  GSM_ModemRegisterPutFunction(&modem, put);
  GSM_ModemRegisterBluetoothCallback(&modem, BTcallback);

  BtPower_t btpower = {0};
  BtPowerObjectInit(&btpower);
  BtPowerSetupRequest(&btpower, 1);

  OS_LockModem_Expect();
  GSM_ModemLock(&modem);

  OS_WaitIfModemIsNotReady_Expect();
  OS_WaitGuardTimeToPass_Expect();
  OS_LockParser_Expect();
  OS_UnlockParser_Expect();
  OS_WaitForResponseWithTimeout_ExpectAnyArgsAndReturn(OS_NO_ERROR);
  GSM_ModemExecuteAtCommand(&modem, &btpower.atcmd);

  OS_UnlockModem_Expect();
  GSM_ModemUnlock(&modem);
}

void test_GSM_ModemExecuteAtCommand_Timeout(void)
{
  GSM_Modem_t modem;
  GSM_ModemObjectInit(&modem);
  GSM_ModemRegisterPutFunction(&modem, put);
  GSM_ModemRegisterBluetoothCallback(&modem, BTcallback);

  BtPower_t btpower = {0};
  BtPowerObjectInit(&btpower);
  BtPowerSetupRequest(&btpower, 1);

  OS_LockModem_Expect();
  GSM_ModemLock(&modem);

  OS_WaitIfModemIsNotReady_Expect();
  OS_WaitGuardTimeToPass_Expect();
  OS_LockParser_Expect();
  OS_UnlockParser_Expect();
  OS_WaitForResponseWithTimeout_ExpectAnyArgsAndReturn(OS_TIMEOUT);
  OS_LockParser_Expect();
  OS_UnlockParser_Expect();
  GSM_ModemExecuteAtCommand(&modem, &btpower.atcmd);

  OS_UnlockModem_Expect();
  GSM_ModemUnlock(&modem);

  TEST_ASSERT_EQUAL(AT_CMD_TIMEOUT, BtPowerGetResponseStatus(&btpower));
}

void test_GSM_ModemParse_AtResponseReceived(void)
{
  GSM_Modem_t modem;
  GSM_ModemObjectInit(&modem);
  GSM_ModemRegisterPutFunction(&modem, put);
  GSM_ModemRegisterBluetoothCallback(&modem, BTcallback);

  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);
  BtConnectSetupRequest(&btconnect, 3, 9);

  modem.currentAt = &btconnect.atcmd;

  const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:10,\"HFP(AG)\"\r\n";
  size_t ilen      = strlen(ibuf);

  OS_LockParser_Expect();
  OS_WakeUpThreadWaitingForResponse_Expect();
  OS_UnlockParser_Expect();
  size_t n = GSM_ModemParse(&modem, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(AT_CMD_OK, btconnect.response.status);
  TEST_ASSERT_EQUAL(1, btconnect.response.result.id);
  TEST_ASSERT_EQUAL_STRING("MK-ZHANZHIMIN", btconnect.response.result.name);
  TEST_ASSERT_EQUAL_STRING("00:1a:7d:da:71:10", btconnect.response.result.address);
  TEST_ASSERT_EQUAL_STRING("HFP(AG)", btconnect.response.result.profile);
}

void test_GSM_ModemParse_URCReceived(void)
{
  GSM_Modem_t modem;
  GSM_ModemObjectInit(&modem);
  GSM_ModemRegisterPutFunction(&modem, put);
  GSM_ModemRegisterBluetoothCallback(&modem, BTcallback);

  const char *ibuf = "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:10,\"HFP(AG)\"\r\n";
  size_t ilen      = strlen(ibuf);

  OS_LockParser_Expect();
  BTcallback_Expect(&modem.bluetooth.event);
  OS_UnlockParser_Expect();
  size_t n = GSM_ModemParse(&modem, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen, n);
  TEST_ASSERT_EQUAL(BTCONNECT_CONNECT, modem.bluetooth.event.type);
  TEST_ASSERT_EQUAL_STRING("MK-ZHANZHIMIN", modem.bluetooth.event.payload.connected.name);
  TEST_ASSERT_EQUAL_STRING("00:1a:7d:da:71:10", modem.bluetooth.event.payload.connected.address);
  TEST_ASSERT_EQUAL_STRING("HFP(AG)", modem.bluetooth.event.payload.connected.profile);
}

void test_GSM_ModemParse_ATandURCReceived(void)
{
  GSM_Modem_t modem;
  GSM_ModemObjectInit(&modem);
  GSM_ModemRegisterPutFunction(&modem, put);
  GSM_ModemRegisterBluetoothCallback(&modem, BTcallback);

  BtConnect_t btconnect;
  BtConnectObjectInit(&btconnect);
  BtConnectSetupRequest(&btconnect, 3, 9);

  modem.currentAt = &btconnect.atcmd;

  const char *ibuf =
      "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:10,\"HFP(AG)\"\r\n"
      "\r\n+BTCONNECT: 1,\"MK-ZHANZHIMIN\",00:1a:7d:da:71:10,\"HFP(AG)\"\r\n";
  size_t ilen = strlen(ibuf);

  OS_LockParser_Expect();
  OS_WakeUpThreadWaitingForResponse_ExpectAnyArgs();
  OS_UnlockParser_Expect();
  size_t n = GSM_ModemParse(&modem, ibuf, ilen);

  TEST_ASSERT_EQUAL(ilen / 2, n);
  TEST_ASSERT_EQUAL(AT_CMD_OK, btconnect.response.status);
  TEST_ASSERT_EQUAL(1, btconnect.response.result.id);
  TEST_ASSERT_EQUAL_STRING("MK-ZHANZHIMIN", btconnect.response.result.name);
  TEST_ASSERT_EQUAL_STRING("00:1a:7d:da:71:10", btconnect.response.result.address);
  TEST_ASSERT_EQUAL_STRING("HFP(AG)", btconnect.response.result.profile);

  // Remove current AT command just like GSM_ModemExecuteAtCommand did.
  modem.currentAt = NULL;

  OS_LockParser_Expect();
  BTcallback_Expect(&modem.bluetooth.event);
  OS_UnlockParser_Expect();
  n = GSM_ModemParse(&modem, ibuf + n, ilen - n);

  TEST_ASSERT_EQUAL(ilen / 2, n);
  TEST_ASSERT_EQUAL(BTCONNECT_CONNECT, modem.bluetooth.event.type);
  TEST_ASSERT_EQUAL_STRING("MK-ZHANZHIMIN", modem.bluetooth.event.payload.connected.name);
  TEST_ASSERT_EQUAL_STRING("00:1a:7d:da:71:10", modem.bluetooth.event.payload.connected.address);
  TEST_ASSERT_EQUAL_STRING("HFP(AG)", modem.bluetooth.event.payload.connected.profile);
}