/**
 * @file btpower.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Common/Env.h"
#include "btpaircfg.h"
#include <string.h>


/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define TIMEOUT_IN_MSEC 2000

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
GSM_STATIC size_t BtPaircfgSerialize(void *p, char *obuf, size_t olen)
{
  BtPaircfg_t *obj = (BtPaircfg_t *)p;
  memset(obuf, 0, olen);

  uint8_t mode    = obj->request.mode;
  const char *pin = obj->request.pin;

  size_t n = 0;
  if (20 < olen) {
    char modestr[2] = {0};
    if ((0 == mode) || (1 == mode) || (2 == mode))
      modestr[0] = mode + '0';

    strncpy(obuf, "AT+BTPAIRCFG=", olen - 1);
    strncat(obuf, modestr, olen - 1 - strlen(obuf));

    if (pin && (1 == mode)) {
      strncat(obuf, ",", olen - 1 - strlen(obuf));
      strncat(obuf, pin, olen - 1 - strlen(obuf));
    }
    strncat(obuf, "\r", olen - 1 - strlen(obuf));
    n = strlen(obuf);
  }

  return n;
}

GSM_STATIC size_t BtPaircfgParse(void *p, const char *ibuf, size_t ilen)
{
  BtPaircfg_t *obj          = (BtPaircfg_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;
  size_t n                  = AT_CommandStatusParse(ibuf, ilen, &status);

  size_t offset = 0;

  switch (status) {
  case AT_CMD_OK:
  case AT_CMD_ERROR:
    obj->response.status = status;
    offset               = n;
    break;
  default:
    obj->response.status = AT_CMD_INVALID;
    offset               = 0;
    break;
  }

  return offset;
}

GSM_STATIC void BtPaircfgTimeout(void *p)
{
  BtPaircfg_t *obj     = (BtPaircfg_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void BtPaircfgObjectInit(BtPaircfg_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = BtPaircfgSerialize;
  this->atcmd.parse             = BtPaircfgParse;
  this->atcmd.timeout           = BtPaircfgTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void BtPaircfgSetupRequest(BtPaircfg_t *this, uint8_t mode, const char *pin)
{
  this->request.mode = mode;
  this->request.pin  = pin;
}

AT_Command_t *BtPaircfgGetAtCommand(BtPaircfg_t *this)
{
  return &this->atcmd;
}

BtPaircfg_response_t BtPaircfgGetResponse(BtPaircfg_t *this)
{
  return this->response;
}

AT_CommandStatus_t BtPaircfgGetResponseStatus(BtPaircfg_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
