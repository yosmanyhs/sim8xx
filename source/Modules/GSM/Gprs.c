/**
 * @file Gprs.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Modules/GSM/Gprs.h"
#include "Modules/GSM/Commands/cstt.h"
#include "Modules/GSM/Commands/cifsr.h"
#include "Modules/GSM/Commands/ciicr.h"
#include "Modules/GSM/Commands/cipstart.h"
#include "Modules/GSM/Commands/cipsend.h"
#include "Modules/GSM/Commands/cipshut.h"
#include "Modules/GSM/Commands/cipclose.h"

#include "Utils/Utils.h"

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
void GprsObjectInit(Gprs_t *this, GSM_Modem_t *modem)
{
  this->modem = modem;
  this->notify = NULL;
  this->state = GPRS_STOPPED;
  memset(this->ipaddr, 0, sizeof(this->ipaddr));
  memset(&this->event, 0, sizeof(this->event));
}

bool GprsRegisterEventCallback(Gprs_t *this, GPRS_EventCb_t cb)
{
  bool result = false;

  if (!this->notify && cb) {
    this->notify = cb;
    result       = true;
  }

  return result;
}

bool GprsStart(Gprs_t *this, const char *apn, const char *user, const char *passwd)
{
  if (GPRS_STOPPED != this->state)
    return false;

  Cstt_t cstt;
  CsttObjectInit(&cstt);
  CsttSetupRequest(&cstt, apn, user, passwd);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &cstt.atcmd);
  GSM_ModemUnlock(this->modem);

  if (AT_CMD_OK != CsttGetResponseStatus(&cstt))
    return false;

  Ciicr_t ciicr;
  CiicrObjectInit(&ciicr);
  CiicrSetupRequest(&ciicr);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &ciicr.atcmd);
  GSM_ModemUnlock(this->modem);

  if (AT_CMD_OK != CiicrGetResponseStatus(&cstt))
    return false;

  Cifsr_t cifsr;
  CifsrObjectInit(&cifsr);
  CifsrSetupRequest(&cifsr);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &cifsr.atcmd);
  GSM_ModemUnlock(this->modem);

  bool result = false;
  if (AT_CMD_OK == CifsrGetResponseStatus(&cifsr)) {
    memset(this->ipaddr, 0, sizeof(this->ipaddr));
    strncpy(this->ipaddr, cifsr.response.ipaddr, size0f(this->ipaddr) - 1);
    this->state = GPRS_DISCONNECTED;
    result = true;
  }

  return result;
}

bool GprsOpenTcpPort(Gprs_t *this, const char *host, int32_t port)
{
  if (GPRS_DISCONNECTED != this->state)
    return false;

  Cipstart_t cipstart;
  CipstartObjectInit(&cipstart);
  CipstartSetupRequest(&cipstart, "TCP", host, port);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &cipstart.atcmd);
  GSM_ModemUnlock(this->modem);

  bool result = false;
  if (AT_CMD_OK == CipstartGetResponseStatus(&cipstart)) {
    this->state = GPRS_WAIT_FOR_CONNECT;
    result = true;
  }

  return result;
}

bool GprsSend(Gprs_t *this, const char *data, size_t dlen)
{
  if (GPRS_CONNECTED != this->state)
    return false;

  Cipsend_t cipsend;
  CipsendObjectInit(&cipsend);
  CipsendSetupRequest(&cipsend, data, dlen);
  CipsendSetCommandMode(&cipsend);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &cipsend.atcmd);

  if (AT_CMD_WAIT_FOR_USER_DATA != CipsendGetResponseStatus(&cipsend)) {
    GSM_ModemUnlock(this->modem);
    return false;
  }

  CipsendSetDataMode(&cipsend);
  GSM_ModemExecuteAtCommand(this->modem, &cipsend.atcmd);
  GSM_ModemUnlock(this->modem);

  return (AT_CMD_SEND_OK == CipsendGetResponseStatus(&cipsend));
}

bool GprsCloseTcpPort(Gprs_t *this)
{
  if (GPRS_CONNECTED != this->state)
    return false;

  Cipclose_t cipclose;
  CipcloseObjectInit(&cipclose);
  CipcloseSetupRequest(&cipclose);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &cipclose.atcmd);
  GSM_ModemUnlock(this->modem);

  bool result = false;
  if (AT_CMD_OK == CipcloseGetResponseStatus(&cipclose)) {
    memset(this->ipaddr, 0, sizeof(this->ipaddr));
    this->state = GPRS_DISCONNECTED;
    result = true;
  }

  return result;
}

bool GprsStop(Gprs_t *this)
{
  if (GPRS_DISCONNECTED != this->state)
    return false;

  Cipshut_t cipshut;
  CipshutObjectInit(&cipshut);
  CipshutSetupRequest(&cipshut);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &cipshut.atcmd);
  GSM_ModemUnlock(this->modem);

  bool result = false;
  if (AT_CMD_OK == CipshutGetResponseStatus(&cipshut)) {
    this->state = GPRS_STOPPED;
    result = true;
  }

  return result;
}

void GprsHandleCipstartURC(Gprs_t *gprs, CipstartURC_t *urc)
{
  switch(urc->type) {
  case CIPSTART_ALREADY_CONNECT: {
    gprs->event.type = GPRS_ALREADY_CONNECTED;
    break;
  }
  case CIPSTART_CONNECT_OK: {
    gprs->event.type = GPRS_CONNECT_OK;
    break;
  }
  case CIPSTART_CONNECT_FAIL: {
    gprs->event.type = GPRS_CONNECT_FAIL;
    gprs->event.payload.fail.state = urc->payload.fail.state;
    break;
  }
  default: {
    break;
  }
  }

  if (gprs->notify && (GPRS_NO_EVENT != gprs->event.type)) {
    gprs->notify(&gprs->event);
  }
}

void GprsHandleCipsendURC(Gprs_t *gprs, CipsendURC_t *urc)
{
  switch(urc->type) {
  case CIPSEND_SEND_OK: {
    gprs->event.type = GPRS_SEND_OK;
    break;
  }
  case CIPSEND_SEND_FAIL: {
    gprs->event.type = GPRS_SEND_FAIL;
    break;
  }
  default: {
    break;
  }
  }

  if (gprs->notify && (GPRS_NO_EVENT != gprs->event.type)) {
    gprs->notify(&gprs->event);
  }
}

size_t GprsURCParse(void *p, const char *ibuf, size_t ilen)
{
  Gprs_t *pgprs = (Gprs_t*)p;
  size_t offset = 0;

  if (CipstartIsURC(ibuf, ilen)) {
    CipstartURC_t urc;
    offset = CipstartParseURC(&urc, ibuf, ilen);
    GprsHandleCipstartURC(pgprs, &urc);
  } else if (CipsendIsURC(ibuf, ilen)) {
    CipsendURC_t urc;
    offset = CipsendParseURC(&urc, ibuf, ilen);
    GprsHandleCipsendURC(pgprs, &urc);
  } else if (GSM_UtilsMatch(ibuf, "\r\n+PDP: DEACT\r\n", ilen)) {
    pgprs->event.type = GPRS_DISCONNECTED;
    if (pgprs->notify) {
      pgprs->notify(&pgprs->event);
  } else if (GSM_UtilsMatch(ibuf, "\r\nCLOSED\r\n", ilen)) {
    pgprs->event.type = GPRS_CONNECTION_CLOSED;
    if (pgprs->notify) {
      pgprs->notify(&pgprs->event);
  } else {
    pgprs->event.type = GPRS_NO_EVENT;
  }

  return offset;
}

/****************************** END OF FILE **********************************/
