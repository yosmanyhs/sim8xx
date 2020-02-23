/**
 * @file cstt.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "cstt.h"
#include "Common/Env.h"
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
GSM_STATIC size_t CsttSerialize(void *p, char *obuf, size_t olen)
{
  Cstt_t *obj = (Cstt_t *)p;
  
  memset(obuf, 0, olen);

  strncpy(obuf, "AT+CSTT=\"", olen - 1);
  strncat(obuf, obj->request.apn, olen - 1 - strlen(obuf));
  if (obj->request.user && obj->request.passwd) {
    strncat(obuf, "\",\"", olen - 1 - strlen(obuf));
    strncat(obuf, obj->request.user, olen - 1 - strlen(obuf));
    strncat(obuf, "\",\"", olen - 1 - strlen(obuf));
    strncat(obuf, obj->request.passwd, olen - 1 - strlen(obuf));
  }

  strncat(obuf, "\"\r", olen - 1 - strlen(obuf));

  return strlen(obuf);
}

GSM_STATIC size_t CsttParse(void *p, const char *ibuf, size_t ilen)
{
  Cstt_t *obj               = (Cstt_t *)p;
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

GSM_STATIC void CsttTimeout(void *p)
{
  Cstt_t *obj = (Cstt_t *)p; 
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CsttObjectInit(Cstt_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = CsttSerialize;
  this->atcmd.parse     = CsttParse;
  this->atcmd.timeout   = CsttTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void CsttSetupRequest(Cstt_t *this, const char *apn, const char *user, const char *passwd)
{
  this->request.apn = apn;
  this->request.user = user;
  this->request.passwd = passwd;
}

AT_Command_t *CsttGetAtCommand(Cstt_t *this)
{
  return &this->atcmd;
}

Cstt_response_t CsttGetResponse(Cstt_t *this)
{
  return this->response;
}

AT_CommandStatus_t CsttGetResponseStatus(Cstt_t *this)
{
  return this->response.status;  
}

/****************************** END OF FILE **********************************/
