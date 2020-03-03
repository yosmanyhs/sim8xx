/**
 * @file Modem.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Modem.h"
#include "Commands/at.h"
#include "Commands/atcfun.h"
#include "Commands/atcpin.h"
#include "Commands/ate.h"
#include "Interface/Os.h"
#include "Utils/Utils.h"
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
size_t GSM_ModemURCParse(GSM_Modem_t *this, const char *ibuf, size_t ilen)
{
  size_t offset = 0;

  if (AtCfunIsURC(ibuf, ilen)) {
    AtCfunURC_t urc = {0};
    offset          = AtCfunParseURC(&urc, ibuf, ilen);
  } else if (AtCpinIsURC(ibuf, ilen)) {
    AtCpinURC_t urc = {0};
    offset          = AtCpinParseURC(&urc, ibuf, ilen);

    if (this->notify) {
      this->event.type = MODEM_EVENT_CPIN;
      switch (urc.payload.info.code) {
      case ATCPIN_NOT_INSERTED: {
        this->event.payload.cpin.status = CPIN_NOT_INSERTED;
        break;
      }
      case ATCPIN_READY: {
        this->event.payload.cpin.status = CPIN_READY;
        break;
      }
      case ATCPIN_SIM_PIN:
      case ATCPIN_PH_SIM_PIN:
      case ATCPIN_SIM_PIN2: {
        this->event.payload.cpin.status = CPIN_PIN_REQUIRED;
        break;
      }
      case ATCPIN_SIM_PUK:
      case ATCPIN_PH_SIM_PUK:
      case ATCPIN_SIM_PUK2: {
        this->event.payload.cpin.status = CPIN_PUK_REQUIRED;
        break;
      }
      case ATCPIN_INVALID_CODE:
      default: {
        this->event.payload.cpin.status = CPIN_INVALID_STATUS;
        break;
      }
      }
      this->notify(&this->event);
    }
  } else if (GSM_UtilsBeginsWith(ibuf, "\r\nCall Ready\r\n")) {
    this->status.callready = 1;
    offset                 = 14;
    if (this->status.callready && this->status.smsready) {
      if (this->notify) {
        this->event.type = MODEM_EVENT_SIM_UNLOCKED;
        this->notify(&this->event);
      }
    }
  } else if (GSM_UtilsBeginsWith(ibuf, "\r\nSms Ready\r\n")) {
    this->status.smsready = 1;
    offset                = 13;
    if (this->status.callready && this->status.smsready) {
      if (this->notify) {
        this->event.type = MODEM_EVENT_SIM_UNLOCKED;
        this->notify(&this->event);
      }
    }
  } else if (GSM_UtilsBeginsWith(ibuf, "\r\nRDY\r\n")) {
    OS_ModemIsReady();
    this->status.ready = 1;
    offset = 7;
  } else {
    offset = 0;
  }

  return offset;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void GSM_ModemObjectInit(GSM_Modem_t *this)
{
  this->currentAt        = NULL;
  this->put              = NULL;
  this->notify           = NULL;
  this->status.ready     = 0;
  this->status.callready = 0;
  this->status.smsready  = 0;
  memset(&this->event, 9, sizeof(this->event));
  GSM_BluetoothObjectInit(&this->bluetooth, this);
  GSM_GpsObjectInit(&this->gps, this);
  GSM_IpObjectInit(&this->ip, this);
}

bool GSM_ModemRegisterCallback(GSM_Modem_t *this, GSM_ModemCb_t cb)
{
  bool result = false;

  if (!this->notify && cb) {
    this->notify = cb;
    result       = true;
  }

  return result;
}

bool GSM_ModemRegisterPutFunction(GSM_Modem_t *this, GSM_SerialPut_t put)
{
  bool result = false;

  if (!this->put && put) {
    this->put = put;
    result    = true;
  }

  return result;
}

bool GSM_ModemRegisterBluetoothCallback(GSM_Modem_t *this, GSM_BluetoothCb_t cb)
{
  return GSM_BluetoothRegisterCallback(&this->bluetooth, cb);
}

bool GSM_ModemRegisterIpCallback(GSM_Modem_t *this, GSM_IpCb_t cb)
{
  return GSM_IpRegisterCallback(&this->ip, cb);
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

bool GSM_ModemUnlockSIMCard(GSM_Modem_t *this, const char *pin)
{
#if 0  
  AtCfun_t cfun;
  AtCfunObjectInit(&cfun);
  AtCfunSetupRequest(&cfun, 1, 0);
  GSM_ModemLock(this);
  GSM_ModemExecuteAtCommand(this, &cfun.atcmd);
  GSM_ModemUnlock(this);

  if (AT_CMD_OK != AtCfunGetResponseStatus(&cfun))
    return false;
#endif
  AtCpin_t cpin;
  AtCpinObjectInit(&cpin);
  AtCpinSetupRequest(&cpin, pin, NULL);
  GSM_ModemLock(this);
  GSM_ModemExecuteAtCommand(this, &cpin.atcmd);
  GSM_ModemUnlock(this);

  return (AT_CMD_OK == AtCpinGetResponseStatus(&cpin));
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
    offset = GSM_IpURCParse(&this->ip, ibuf, ilen);
  }

  if (0 == offset) {
    offset = GSM_ModemURCParse(this, ibuf, ilen);
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

bool GSM_ModemIpSetup(GSM_Modem_t *this, const char *apn)
{
  return GSM_IpSetup(&this->ip, apn);
}

bool GSM_ModemIpOpen(GSM_Modem_t *this)
{
  return GSM_IpOpen(&this->ip);
}

bool GSM_ModemIpClose(GSM_Modem_t *this)
{
  return GSM_IpClose(&this->ip);
}

bool GSM_ModemIpHttpStart(GSM_Modem_t *this)
{
  return GSM_IpHttpStart(&this->ip);
}

bool GSM_ModemIpHttpGet(GSM_Modem_t *this, const char *url)
{
  return GSM_IpHttpGet(&this->ip, url);
}

bool GSM_ModemIpHttpStop(GSM_Modem_t *this)
{
  return GSM_IpHttpStop(&this->ip);
}

/****************************** END OF FILE **********************************/
