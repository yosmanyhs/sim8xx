/**
 * @file btsppurc.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "btsppurc.h"
#include "Common/Env.h"
#include <stdio.h>
#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define TIMEOUT_IN_MSEC 10000

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
GSM_STATIC size_t BtsppurcSerialize(void *p, char *obuf, size_t olen)
{
  Btsppurc_t *obj = (Btsppurc_t*)p;
  
  memset(obuf, 0, olen);

  if (15 < olen) {
    strncpy(obuf, "AT+BTSPPURC=", olen - 2);
    strncat(obuf, obj->request.mode == 1 ? "1" : "0", olen - 1 - strlen(obuf));
    strncat(obuf, "\r", olen - 1 - strlen(obuf));
  }

  return strlen(obuf);
}

GSM_STATIC size_t BtsppurcParse(void *p, const char *ibuf, size_t ilen)
{
  Btsppurc_t *obj = (Btsppurc_t*)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t n = AT_CommandStatusParse(ibuf, ilen, &status);

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

GSM_STATIC void BtsppurcTimeout(void *p)
{
  Btsppurc_t *obj = (Btsppurc_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void BtsppurcObjectInit(Btsppurc_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = BtsppurcSerialize;
  this->atcmd.parse             = BtsppurcParse;
  this->atcmd.timeout           = BtsppurcTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void BtsppurcSetupRequest(Btsppurc_t *this, uint32_t mode)
{
  this->request.mode = mode;
}

AT_Command_t *BtsppurcGetAtCommand(Btsppurc_t *this)
{
  return &this->atcmd;
}

Btsppurc_Response_t BtsppurcGetResponse(Btsppurc_t *this)
{
  return this->response;
}

AT_CommandStatus_t BtsppurcGetResponseStatus(Btsppurc_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
