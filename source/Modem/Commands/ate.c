/**
 * @file ate.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "ate.h"
#include "Common/Env.h"
#include <string.h>
#include <stdio.h>

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
GSM_STATIC size_t AteSerialize(void *p, char *obuf, size_t olen)
{
  Ate_t *obj = (Ate_t *)p;
  memset(obuf, 0, olen);

  if (6 < olen) {
    strncpy(obuf, "ATE", olen - 3);
    const char *str = (0 == obj->request.mode) ? "0\r" : "1\r";
    strncat(obuf, str, olen - 1 - strlen(obuf));
  }

  return strlen(obuf);
}

GSM_STATIC size_t AteParse(void *p, const char *ibuf, size_t ilen)
{
  Ate_t *obj             = (Ate_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t n = 0;

  if (6 <= ilen) {
    char echo[10] = {0};
    size_t elen = AteSerialize(p, echo, sizeof(echo));

    if (0 == strncasecmp(ibuf, echo, elen))
      n += elen;

    n += AT_CommandStatusParse(ibuf + n, ilen - n, &status);
  }

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

GSM_STATIC void AteTimeout(void *p)
{
  Ate_t *obj = (Ate_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void AteObjectInit(Ate_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = AteSerialize;
  this->atcmd.parse     = AteParse;
  this->atcmd.timeout   = AteTimeout;
  this->atcmd.timeoutInMilliSec   = TIMEOUT_IN_MSEC;
}

void AteSetupRequest(Ate_t *this, uint8_t mode)
{
  this->request.mode = mode;
}

AT_Command_t *AteGetAtCommand(Ate_t *this)
{
  return &this->atcmd;
}

Ate_Response_t AteGetResponse(Ate_t *this)
{
  return this->response;
}

AT_CommandStatus_t AteGetResponseStatus(Ate_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
