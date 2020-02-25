/**
 * @file cipclose.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "cipclose.h"
#include "Common/Env.h"
#include "Utils/Utils.h"
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
GSM_STATIC size_t CipcloseSerialize(void *p, char *obuf, size_t olen)
{
  (void)p;
  
  memset(obuf, 0, olen);

  strncpy(obuf, "AT+CIPCLOSE\r", olen);

  return strlen(obuf);
}

GSM_STATIC size_t CipcloseParse(void *p, const char *ibuf, size_t ilen)
{
  Cipclose_t *obj = (Cipclose_t*)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t n = AT_CommandStatusParse(ibuf, ilen, &status);

  size_t offset = 0;
  if (AT_CMD_ERROR == status) {
    obj->response.status = AT_CMD_ERROR;
    offset = n;
  } else if (GSM_UtilsBeginsWith(ibuf, "\r\nCLOSE OK\r\n")) {
    obj->response.status = AT_CMD_OK;
    offset = 12;
  } else {
    obj->response.status = AT_CMD_INVALID;
    offset = 0;
  }

  return offset;
}

GSM_STATIC void CipcloseTimeout(void *p)
{
  Cipclose_t *obj = (Cipclose_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CipcloseObjectInit(Cipclose_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = CipcloseSerialize;
  this->atcmd.parse             = CipcloseParse;
  this->atcmd.timeout           = CipcloseTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void CipcloseSetupRequest(Cipclose_t *this)
{
  (void)this;
}

AT_Command_t *CipcloseGetAtCommand(Cipclose_t *this)
{
  return &this->atcmd;
}

Cipclose_Response_t CipcloseGetResponse(Cipclose_t *this)
{
  return this->response;
}

AT_CommandStatus_t CipcloseGetResponseStatus(Cipclose_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
