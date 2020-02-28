/**
 * @file Ip.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Ip.h"
#include "Commands/sapbr.h"
#include "Commands/httpinit.h"
#include "Commands/httppara.h"
#include "Commands/httpaction.h"
#include "Commands/httpterm.h"
#include "Modem/Modem.h"

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
HttpMethod_t GSM_IpDecodeHttpMethod(HttpactionMethod_t method)
{
  HttpMethod_t m;
  switch(method) {
    case HTTPACTION_GET: {
      m = IP_HTTP_METHOD_GET;
      break;
    }
    case HTTPACTION_POST: {
      m = IP_HTTP_METHOD_SET;
      break;
    }
    case HTTPACTION_HEAD: {
      m = IP_HTTP_METHOD_HEAD;
      break;
    }
    default: {
      m = IP_HTTP_METHOD_INVALID;
      break;
    }
  }

  return m;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void GSM_IpObjectInit(GSM_Ip_t *this, GSM_Modem_t *modem)
{
  memset(this, 0, sizeof(*this));
  this->modem = modem;
}

bool GSM_IpRegisterCallback(GSM_Ip_t *this, GSM_IpCb_t cb)
{
  bool result = false;

  if (!this->notify && cb) {
    this->notify = cb;
    result       = true;
  }

  return result;
}

bool GSM_IpSetup(GSM_Ip_t *this, const char *apn)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestSetConnTypeGprs(&sapbr, 1);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &sapbr.atcmd);
  GSM_ModemUnlock(this->modem);

  if (AT_CMD_OK != SapbrGetResponseStatus(&sapbr))
    return false;

  SapbrObjectInit(&sapbr);
  SapbrSetupRequestSetApn(&sapbr, 1, apn);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &sapbr.atcmd);
  GSM_ModemUnlock(this->modem);

  return (AT_CMD_OK == SapbrGetResponseStatus(&sapbr));
}

bool GSM_IpOpen(GSM_Ip_t *this)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestOpenContext(&sapbr, 1);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &sapbr.atcmd);
  GSM_ModemUnlock(this->modem);

  return (AT_CMD_OK == SapbrGetResponseStatus(&sapbr));
}

bool GSM_IpClose(GSM_Ip_t *this)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestCloseContext(&sapbr, 1);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &sapbr.atcmd);
  GSM_ModemUnlock(this->modem);

  return (AT_CMD_OK == SapbrGetResponseStatus(&sapbr));
}

bool GSM_IpHttpStart(GSM_Ip_t *this)
{
  Httpinit_t httpinit;
  HttpinitObjectInit(&httpinit);
  HttpinitSetupRequest(&httpinit);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &httpinit.atcmd);
  GSM_ModemUnlock(this->modem);

  if (AT_CMD_OK != HttpinitGetResponseStatus(&httpinit))
    return false;

  Httppara_t httppara;
  HttpparaObjectInit(&httppara);
  HttpparaSetupRequestSetCid(&httppara, "1");
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &httppara.atcmd);
  GSM_ModemUnlock(this->modem);

  return (AT_CMD_OK == HttpparaGetResponseStatus(&httppara));
}

bool GSM_IpHttpGet(GSM_Ip_t *this, const char *url)
{
  Httppara_t httppara;
  HttpparaObjectInit(&httppara);
  HttpparaSetupRequestSetUrl(&httppara, url);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &httppara.atcmd);
  GSM_ModemUnlock(this->modem);

  if (AT_CMD_OK != HttpparaGetResponseStatus(&httppara))
    return false;

  Httpaction_t httpaction;
  HttpactionObjectInit(&httpaction);
  HttpactionSetupRequest(&httpaction, HTTPACTION_GET);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &httpaction.atcmd);
  GSM_ModemUnlock(this->modem);

  return (AT_CMD_OK == HttpactionGetResponseStatus(&httpaction));
}

bool GSM_IpHttpStop(GSM_Ip_t *this)
{
  Httpterm_t httpterm;
  HttptermObjectInit(&httpterm);
  HttptermSetupRequest(&httpterm);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &httpterm.atcmd);
  GSM_ModemUnlock(this->modem);

  return (AT_CMD_OK == HttptermGetResponseStatus(&httpterm));
}

size_t GSM_IpURCParse(void *p, const char *ibuf, size_t ilen)
{
  GSM_Ip_t *pip = (GSM_Ip_t *)p;
  size_t offset = 0;

  if (SapbrIsURC(ibuf, ilen)) {
    SapbrURC_t urc;
    offset = SapbrParseURC(&urc, ibuf, ilen);
    if (0 < offset) {
      pip->event.type                    = IP_EVENT_NET_DISCONNECTED;
      pip->event.payload.disconnected.id = urc.payload.deact.id;

      if (pip->notify) {
        pip->notify(&pip->event);
      }
    }
  } else if (HttpactionIsURC(ibuf, ilen)) {
    HttpactionURC_t urc;
    offset = HttpactionParseURC(&urc, ibuf, ilen);

    if (0 < offset) {
      pip->event.type                          = IP_EVENT_HTTP_ACTION;
      pip->event.payload.httpaction.method     = GSM_IpDecodeHttpMethod(urc.method);
      pip->event.payload.httpaction.httpStatus = urc.httpStatus;
      pip->event.payload.httpaction.dataLength = urc.dataLength;

      if (pip->notify) {
        pip->notify(&pip->event);
      }
    }
  }

  return offset;
}

/****************************** END OF FILE **********************************/
