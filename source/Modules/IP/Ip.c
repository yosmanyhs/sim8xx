/**
 * @file Ip.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Ip.h"
#include "Commands/sapbr.h"
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

bool GSM_IpSetup(GSM_Ip_t *this, int32_t id, const char *apn)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestSetConnTypeGprs(&sapbr, id);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &sapbr.atcmd);
  GSM_ModemUnlock(this->modem);

  if (AT_CMD_OK != SapbrGetResponseStatus(&sapbr))
    return false;

  SapbrObjectInit(&sapbr);
  SapbrSetupRequestSetApn(&sapbr, id, apn);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &sapbr.atcmd);
  GSM_ModemUnlock(this->modem);

  return (AT_CMD_OK == SapbrGetResponseStatus(&sapbr));
}

bool GSM_IpOpen(GSM_Ip_t *this, int32_t id)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestOpenContext(&sapbr, id);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &sapbr.atcmd);
  GSM_ModemUnlock(this->modem);

  return (AT_CMD_OK == SapbrGetResponseStatus(&sapbr));
}

bool GSM_IpClose(GSM_Ip_t *this, int32_t id)
{
  Sapbr_t sapbr;
  SapbrObjectInit(&sapbr);
  SapbrSetupRequestCloseContext(&sapbr, id);
  GSM_ModemLock(this->modem);
  GSM_ModemExecuteAtCommand(this->modem, &sapbr.atcmd);
  GSM_ModemUnlock(this->modem);

  return (AT_CMD_OK == SapbrGetResponseStatus(&sapbr));
}

size_t GSM_IpURCParse(void *p, const char *ibuf, size_t ilen)
{
  GSM_Ip_t *pip = (GSM_Ip_t *)p;
  size_t offset = 0;

  if (SapbrIsURC(ibuf, ilen)) {
    SapbrURC_t urc;
    offset = SapbrParseURC(&urc, ibuf, ilen);
    
    pip->event.type = IP_EVENT_NET_DISCONNECTED;
    pip->event.payload.disconnected.id = urc.payload.deact.id;
    
    if (pip->notify) {
      pip->notify(&pip->event);
    }
  }

  return offset;
}

/****************************** END OF FILE **********************************/
