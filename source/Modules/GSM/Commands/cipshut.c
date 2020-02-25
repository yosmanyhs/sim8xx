/**
 * @file cipshut.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "cipshut.h"
#include "Common/Env.h"
#include "Utils/Utils.h"
#include <stdio.h>
#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define TIMEOUT_IN_MSEC 65000

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
GSM_STATIC size_t CipshutSerialize(void *p, char *obuf, size_t olen)
{
  (void)p;
  
  memset(obuf, 0, olen);

  strncpy(obuf, "AT+CIPSHUT\r", olen);

  return strlen(obuf);
}

GSM_STATIC size_t CipshutParse(void *p, const char *ibuf, size_t ilen)
{
  Cipshut_t *obj = (Cipshut_t*)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t n = AT_CommandStatusParse(ibuf, ilen, &status);

  size_t offset = 0;
  if (AT_CMD_ERROR == status) {
    obj->response.status = AT_CMD_ERROR;
    offset = n;
  } else if (GSM_UtilsBeginsWith(ibuf, "\r\nSHUT OK\r\n")) {
    obj->response.status = AT_CMD_OK;
    offset = 11;
  } else {
    obj->response.status = AT_CMD_INVALID;
    offset = 0;
  }

  return offset;
}

GSM_STATIC void CipshutTimeout(void *p)
{
  Cipshut_t *obj = (Cipshut_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CipshutObjectInit(Cipshut_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = CipshutSerialize;
  this->atcmd.parse             = CipshutParse;
  this->atcmd.timeout           = CipshutTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void CipshutSetupRequest(Cipshut_t *this)
{
  (void)this;
}

AT_Command_t *CipshutGetAtCommand(Cipshut_t *this)
{
  return &this->atcmd;
}

Cipshut_Response_t CipshutGetResponse(Cipshut_t *this)
{
  return this->response;
}

AT_CommandStatus_t CipshutGetResponseStatus(Cipshut_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
