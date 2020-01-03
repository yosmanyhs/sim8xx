/**
 * @file at.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "at.h"
#include "Env.h"
#include <string.h>
#include <stdio.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define TIMEOUT_IN_SEC 2

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
GSM_STATIC size_t AtSerialize(void *p, char *obuf, size_t length)
{
  memset(obuf, 0, length);

  (void)p;
  size_t n      = 0;
  if (2 < length) {
    strncpy(obuf, "AT", length);
    n = strlen(obuf);
  }

  return n;
}

GSM_STATIC size_t AtParse(void *p, const char *ibuf, size_t length)
{
  At_t *obj             = (At_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t n = 0;
  if (3 < length) {
    if (0 == strncasecmp(ibuf, "AT\r", 3)) {
      n += 3;
    }
    
    n += AT_CommandStatusParse(ibuf + n, length - n, &status);
  } 

  if (n && (AT_CMD_OK == status)) {
    obj->response.status = status;
  } else {
    n = 0;
  }

  return n;
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
  this->atcmd.timeoutInSec   = TIMEOUT_IN_SEC;
}

void AtSetupRequest(At_t *this)
{
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
