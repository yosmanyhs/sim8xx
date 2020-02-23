/**
 * @file ciicr.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "ciicr.h"
#include "Common/Env.h"
#include <stdio.h>
#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define TIMEOUT_IN_MSEC 85000

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
GSM_STATIC size_t CiicrSerialize(void *p, char *obuf, size_t olen)
{
  (void)p;
  memset(obuf, 0, olen);

  if (10 < olen) {
    strncpy(obuf, "AT+CIICR\r", olen - 1);
  }

  return strlen(obuf);
}

GSM_STATIC size_t CiicrParse(void *p, const char *ibuf, size_t ilen)
{
  Ciicr_t *obj              = (Ciicr_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t n = AT_CommandStatusParse(ibuf, ilen, &status);

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

GSM_STATIC void CiicrTimeout(void *p)
{
  Ciicr_t *obj         = (Ciicr_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CiicrObjectInit(Ciicr_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = CiicrSerialize;
  this->atcmd.parse             = CiicrParse;
  this->atcmd.timeout           = CiicrTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void CiicrSetupRequest(Ciicr_t *this)
{
  (void)this;
}

AT_Command_t *CiicrGetAtCommand(Ciicr_t *this)
{
  return &this->atcmd;
}

Ciicr_Response_t CiicrGetResponse(Ciicr_t *this)
{
  return this->response;
}

AT_CommandStatus_t CiicrGetResponseStatus(Ciicr_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
