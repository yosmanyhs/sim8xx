/**
 * @file at.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "at.h"
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
GSM_STATIC size_t AtSerialize(void *p, char *obuf, size_t olen)
{
  (void)p;
  memset(obuf, 0, olen);

  if (3 < olen) {
    strncpy(obuf, "AT\r", olen - 1);
  }

  return strlen(obuf);
}

GSM_STATIC size_t AtParse(void *p, const char *ibuf, size_t ilen)
{
  At_t *obj             = (At_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t n = 0;
  if (3 < ilen) {
    if (0 == strncasecmp(ibuf, "AT\r", 3)) {
      n += 3;
    }
    
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

GSM_STATIC void AtTimeout(void *p)
{
  At_t *obj = (At_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void AtObjectInit(At_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = AtSerialize;
  this->atcmd.parse     = AtParse;
  this->atcmd.timeout   = AtTimeout;
  this->atcmd.timeoutInMilliSec   = TIMEOUT_IN_MSEC;
}

void AtSetupRequest(At_t *this)
{
  (void)this;
}

AT_Command_t *AtGetAtCommand(At_t *this)
{
  return &this->atcmd;
}

At_Response_t AtGetResponse(At_t *this)
{
  return this->response;
}

AT_CommandStatus_t AtGetResponseStatus(At_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
