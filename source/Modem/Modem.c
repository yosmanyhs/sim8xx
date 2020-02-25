/**
 * @file Modem.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Modem.h"
#include "Commands/at.h"
#include "Commands/ate.h"
#include "Interface/Os.h"

#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void GSM_ModemObjectInit(GSM_Modem_t *this)
{
  this->currentAt = NULL;
  this->put       = NULL;
  GSM_BluetoothObjectInit(&this->bluetooth, this);
  GSM_GpsObjectInit(&this->gps, this);
  GSM_GsmObjectInit(&this->gsm, this);
}

bool GSM_ModemRegisterPutFunction(GSM_Modem_t *this, GSM_SerialPut_t put)
{
  bool result = false;

  if (!this->put) {
    this->put = put;
    result    = true;
  }

  return result;
}

bool GSM_ModemRegisterBluetoothCallback(GSM_Modem_t *this, GSM_BluetoothCb_t cb)
{
  return GSM_BluetoothRegisterCallback(&this->bluetooth, cb);
}

bool GSM_ModemIsAlive(GSM_Modem_t *this)
{
  At_t at;
  AtObjectInit(&at);
  AtSetupRequest(&at);
  GSM_ModemLock(this);

  AT_Command_t *atcmd = AtGetAtCommand(&at);
  char msg[10]        = {0};
  size_t n            = atcmd->serialize(atcmd->obj, msg, sizeof(msg));

  size_t i;
  for (i = 0; i < n; ++i) {
    this->put(msg[i]);
  }

  OS_LockParser();
  this->currentAt = atcmd;
  OS_UnlockParser();

  OS_Error_t error = OS_WaitForResponseWithTimeout(atcmd->timeoutInMilliSec);

  OS_LockParser();
  this->currentAt = NULL;
  OS_UnlockParser();

  if (OS_TIMEOUT == error) {
    atcmd->timeout(atcmd->obj);
  }

  GSM_ModemUnlock(this);

  bool isAlive = (AT_CMD_OK == AtGetResponseStatus(&at));
  if (!isAlive)
    OS_ModemIsNotReady();

  return isAlive;
}

bool GSM_ModemDisableEcho(GSM_Modem_t *this)
{
  Ate_t ate;
  AteObjectInit(&ate);
  AteSetupRequest(&ate, 0);
  GSM_ModemLock(this);
  GSM_ModemExecuteAtCommand(this, AteGetAtCommand(&ate));
  GSM_ModemUnlock(this);

  return (AT_CMD_OK == AteGetResponseStatus(&ate));
}

void GSM_ModemLock(GSM_Modem_t *this)
{
  (void)this;
  OS_LockModem();
}

void GSM_ModemUnlock(GSM_Modem_t *this)
{
  (void)this;
  OS_UnlockModem();
}

void GSM_ModemExecuteAtCommand(GSM_Modem_t *this, AT_Command_t *atcmd)
{
  OS_WaitIfModemIsNotReady();
  OS_WaitGuardTimeToPass();

  char obuf[512] = {0};
  size_t olen    = sizeof(obuf);
  size_t n       = atcmd->serialize(atcmd->obj, obuf, olen);

  size_t i;
  for (i = 0; i < n; ++i) {
    this->put(obuf[i]);
  }

  OS_LockParser();
  this->currentAt = atcmd;
  OS_UnlockParser();

  OS_Error_t error = OS_WaitForResponseWithTimeout(atcmd->timeoutInMilliSec);

  if (OS_TIMEOUT == error) {
    OS_LockParser();
    this->currentAt = NULL;
    OS_UnlockParser();
    atcmd->timeout(atcmd->obj);
  }
}

size_t GSM_ModemParse(GSM_Modem_t *this, const char *ibuf, size_t ilen)
{
  OS_LockParser();

  size_t offset = 0;
  if (this->currentAt) {
    AT_Parse_t parser = this->currentAt->parse;
    offset            = parser(this->currentAt->obj, ibuf, ilen);
    if (offset) {
      OS_WakeUpThreadWaitingForResponse();
      this->currentAt = NULL;
    }
  }

  if (0 == offset) {
    offset = GSM_BluetoothURCParse(&this->bluetooth, ibuf, ilen);
  }

  if (0 == offset) {
    offset = GSM_GsmURCParse(&this->gsm, ibuf, ilen);
  }

  if (0 == offset) {
    if (0 == strncasecmp(ibuf, "\r\nRDY\r\n", 7)) {
      offset = strlen("\r\nRDY\r\n");
      OS_ModemIsReady();
    }
  }

  OS_UnlockParser();

  return offset;
}

bool GSM_ModemBluetoothSetup(GSM_Modem_t *this, const char *name, const char *pin)
{
  return GSM_BluetoothSetup(&this->bluetooth, name, pin);
}

bool GSM_ModemBluetoothStart(GSM_Modem_t *this)
{
  return GSM_BluetoothStart(&this->bluetooth);
}

bool GSM_ModemBluetoothStop(GSM_Modem_t *this)
{
  return GSM_BluetoothStop(&this->bluetooth);
}

bool GSM_ModemBluetoothAcceptConnection(GSM_Modem_t *this)
{
  return GSM_BluetoothAcceptConnection(&this->bluetooth);
}

bool GSM_ModemBluetoothSendSppData(GSM_Modem_t *this, const char data[], size_t length)
{
  return GSM_BluetoothSendSPPData(&this->bluetooth, data, length);
}

bool GSM_ModemGpsStart(GSM_Modem_t *this)
{
  return GSM_GpsStart(&this->gps);
}

bool GSM_ModemGpsStop(GSM_Modem_t *this)
{
  return GSM_GpsStop(&this->gps);
}

bool GSM_ModemGpsRead(GSM_Modem_t *this, GPS_Data_t *data)
{
  return GSM_GpsRead(&this->gps, data);
}

bool GSM_ModemGprsRegisterCallback(GSM_Modem_t *this, GPRS_EventCb_t cb)
{
  return GSM_GsmGprsRegisterCallback(&this->gsm, cb);
}

bool GSM_ModemStart(GSM_Modem_t *this, const char *pin)
{
  return GSM_GsmStart(&this->gsm, pin);
}

bool GSM_ModemStop(GSM_Modem_t *this)
{
  return GSM_GsmStop(&this->gsm);
}

bool GSM_ModemGprsStart(GSM_Modem_t *this, const char *apn, const char *user, const char *passwd)
{
  return GSM_GsmGprsStart(&this->gsm, apn, user, passwd);
}

bool GSM_ModemGprsOpenTcpPort(GSM_Modem_t *this, const char *host, int32_t port)
{
  return GSM_GsmGprsOpenTcpPort(&this->gsm, host, port);
}

bool GSM_ModemGprsSend(GSM_Modem_t *this, const char *data, size_t dlen)
{
  return GSM_GsmGprsSend(&this->gsm, data, dlen);
}

bool GSM_ModemGprsCloseTcpPort(GSM_Modem_t *this)
{
  return GSM_GsmGprsCloseTcpPort(&this->gsm);
}

bool GSM_ModemGprsStop(GSM_Modem_t *this)
{
  return GSM_GsmGprsStop(&this->gsm);
}

/****************************** END OF FILE **********************************/
