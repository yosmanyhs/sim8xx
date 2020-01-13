/**
 * @file btacpt.c
 * @brief
 */
/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Common/Env.h"
#include "btacpt.h"


#include <stdio.h>
#include <string.h>


#include "Utils/Utils.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define TIMEOUT_IN_MSEC 5000

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
GSM_STATIC size_t BtAcptSerialize(void *p, char *obuf, size_t olen)
{
  BtAcpt_t *obj = (BtAcpt_t *)p;
  memset(obuf, 0, olen);

  size_t n = 0;
  if (15 < olen) {
    strncpy(obuf, "AT+BTACPT=", olen);
    GSM_UtilsItoA(obuf + 10, olen - 2 - 10, (int)obj->request.mode);
    strncat(obuf, "\r", olen - 1 - strlen(obuf));
    n = strlen(obuf);
  }

  return n;
}

GSM_STATIC size_t BtAcptParse(void *p, const char *ibuf, size_t ilen)
{
  BtAcpt_t *obj             = (BtAcpt_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;
  size_t n                  = AT_CommandStatusParse(ibuf, ilen, &status);

  size_t offset = 0;

  switch (status) {
  case AT_CMD_OK:
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

GSM_STATIC void BtAcptTimeout(void *p)
{
  BtAcpt_t *obj        = (BtAcpt_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void BtAcptObjectInit(BtAcpt_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = BtAcptSerialize;
  this->atcmd.parse             = BtAcptParse;
  this->atcmd.timeout           = BtAcptTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void BtAcptSetupRequest(BtAcpt_t *this, uint8_t mode)
{
  this->request.mode = mode;
}

AT_Command_t *BtAcptGetAtCommand(BtAcpt_t *this)
{
  return &this->atcmd;
}

BtAcpt_response_t BtAcptGetResponse(BtAcpt_t *this)
{
  return this->response;
}

AT_CommandStatus_t BtAcptGetResponseStatus(BtAcpt_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
