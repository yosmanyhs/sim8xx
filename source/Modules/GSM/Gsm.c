/**
 * @file Gsm.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Gsm.h"
#include "Modem/Modem.h"
#include "Modules/GSM/Commands/cfun.h"
#include "Modules/GSM/Commands/cpin.h"

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

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void GSM_GsmObjectInit(Gsm_t *this, GSM_Modem_t *modem)
{
  this->status.callready = 0;
  this->status.smsready = 0;
  this->state = GSM_STATE_INIT;
  this->modem = modem;
  GprsObjectInit(&this->gprs, modem);
}

bool GSM_GsmGprsRegisterCallback(Gsm_t *this, GPRS_EventCb_t cb)
{
  return GprsRegisterEventCallback(&this->gprs, cb); 
}

bool GSM_GsmStart(Gsm_t *this, const char *pin)
{
  if (GSM_STATE_STARTED == this->state)
    return true;

  Cfun_t cfun;
  CfunObjectInit(&cfun);
  CfunSetupRequest(&cfun, 1, 0);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &cfun.atcmd);
  GSM_ModemUnlock(this->modem);

  if (AT_CMD_OK != CfunGetResponseStatus(&cfun))
    return false;

  Cpin_t cpin;
  CpinObjectInit(&cpin);
  CpinSetupRequest(&cpin, pin, NULL);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &cpin.atcmd);
  GSM_ModemUnlock(this->modem);

  bool result = false;
  if (AT_CMD_OK == CpinGetResponseStatus(&cpin)) {
    this->state = GSM_STATE_WAIT_FOR_START;
    result = true;
  }

  return result;
}

bool GSM_GsmStop(Gsm_t *this)
{
  Cfun_t cfun;
  CfunObjectInit(&cfun);
  CfunSetupRequest(&cfun, 0, 0);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &cfun.atcmd);
  GSM_ModemUnlock(this->modem);

  bool result = false;
  if (AT_CMD_OK == CfunGetResponseStatus(&cfun)) {
    this->status.callready = 0;
    this->status.smsready = 0;
    this->state = GSM_STATE_STOPPED;
    result = true;
  }

  return result;
}
bool GSM_GsmGprsStart(Gsm_t *this, const char *apn, const char *user, const char *passwd)
{
  bool result = false;
  if (GSM_STATE_STARTED == this->state)
    result = GprsStart(&this->gprs, apn, user, passwd);

  return result;
}

bool GSM_GsmGprsOpenTcpPort(Gsm_t *this, const char *host, int32_t port)
{
  bool result = false;
  if (GSM_STATE_STARTED == this->state)
    result = GprsOpenTcpPort(&this->gprs, host, port);

  return result;
}

bool GSM_GsmGprsSend(Gsm_t *this, const char *data, size_t dlen)
{
  bool result = false;
  if (GSM_STATE_STARTED == this->state)
    result = GprsSend(&this->gprs, data, dlen);

  return result;
}

bool GSM_GsmGprsCloseTcpPort(Gsm_t *this)
{
  bool result = false;
  if (GSM_STATE_STARTED == this->state)
    result = GprsCloseTcpPort(&this->gprs);

  return result;
}

bool GSM_GsmGprsStop(Gsm_t *this)
{
  bool result = false;
  if (GSM_STATE_STARTED == this->state)
    result = GprsStop(&this->gprs);

  return result;
}

size_t GSM_GsmURCParse(void *p, const char *ibuf, size_t ilen)
{
  Gsm_t *pgsm = (Gsm_t *)p;
  size_t offset = 0;

  offset = GprsURCParse(&pgsm->gprs, ibuf, ilen);
  
  if (0 == offset) {
    CfunURC_t urc;
    offset = CfunParseURC(&urc, ibuf, ilen);
  } 
  
  if (0 == offset) {
    CpinURC_t urc;
    offset = CpinParseURC(&urc, ibuf, ilen);
  }

  if (0 == offset) {
    const char *tag = "\r\nCall ready\r\n";
    if (GSM_UtilsBeginsWith(ibuf, tag)) {
      pgsm->status.callready = 1;
      if (pgsm->status.callready && pgsm->status.smsready) {
        pgsm->state = GSM_STATE_STARTED;
        offset = strlen(tag);
      }
    }
  }

  if (0 == offset) {
    const char *tag = "\r\nSms ready\r\n";
    if (GSM_UtilsBeginsWith(ibuf, tag)) {
      pgsm->status.callready = 1;
      if (pgsm->status.callready && pgsm->status.smsready) {
        pgsm->state = GSM_STATE_STARTED;
        offset = strlen(tag);
      }
    }
  }

  return offset;
}

/****************************** END OF FILE **********************************/
