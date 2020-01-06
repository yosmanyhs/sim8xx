/**
 * @file btacpt.c
 * @brief
 */
/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "btacpt.h"
#include "Common/Env.h"

#include <string.h>
#include <stdio.h>

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
GSM_STATIC size_t BtAcptSerialize(void *p, char *obuf, size_t length)
{
  memset(obuf, 0, length);

  BtAcpt_t *obj = (BtAcpt_t *)p;
  size_t n      = 0;
  if (12 < length) {
    strncpy(obuf, "AT+BTACPT=", length);
    GSM_UtilsItoA(obuf + 10, length - 10, (int)obj->request.mode);
    n = strlen(obuf);
  }

  return n;
}

GSM_STATIC size_t BtAcptParse(void *p, const char *ibuf, size_t length)
{
  BtAcpt_t *obj             = (BtAcpt_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;
  size_t n                  = AT_CommandStatusParse(ibuf, length, &status);

  if (n && (AT_CMD_OK == status)) {
    obj->response.status = status;
  } else {
    n = 0;
  }

  return n;
}

GSM_STATIC void BtAcptTimeout(void *p)
{
  BtAcpt_t *obj = (BtAcpt_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void BtAcptObjectInit(BtAcpt_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = BtAcptSerialize;
  this->atcmd.parse     = BtAcptParse;
  this->atcmd.timeout     = BtAcptTimeout;
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
