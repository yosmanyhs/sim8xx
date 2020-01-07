/**
 * @file btpower.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "bthost.h"
#include "Common/Env.h"
#include <string.h>

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
GSM_STATIC size_t BtHostSerialize(void *p, char *obuf, size_t length)
{
  memset(obuf, 0, length);
  
  BtHost_t *obj = (BtHost_t *)p;
  size_t n      = 0;
  if (28 < length) {
    strncat(obuf, "AT+BTHOST=", length - 1);
    strncat(obuf, obj->request.name, length - 1 - 10);
    n = strlen(obuf);
    strncat(obuf, "\r", length - n);
    n = strlen(obuf);
  }

  return n;
}

GSM_STATIC size_t BtHostParse(void *p, const char *ibuf, size_t length)
{
  BtHost_t *obj             = (BtHost_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;
  size_t n                  = AT_CommandStatusParse(ibuf, length, &status);

  if (n && ((AT_CMD_OK == status) || (AT_CMD_ERROR == status))) {
    obj->response.status = status;
  } else {
    n = 0;
  }

  return n;
}

GSM_STATIC void BtHostTimeout(void *p)
{
  BtHost_t *obj = (BtHost_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void BtHostObjectInit(BtHost_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = BtHostSerialize;
  this->atcmd.parse     = BtHostParse;
  this->atcmd.timeout   = BtHostTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void BtHostSetupRequest(BtHost_t *this, const char *name)
{
  this->request.name = name;
}

AT_Command_t *BtHostGetAtCommand(BtHost_t *this)
{
  return &this->atcmd;
}

BtHost_response_t BtHostGetResponse(BtHost_t *this)
{
  return this->response;
}

AT_CommandStatus_t BtHostGetResponseStatus(BtHost_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
