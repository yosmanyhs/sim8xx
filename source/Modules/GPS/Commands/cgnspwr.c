/**
 * @file cgnspwr.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "cgnspwr.h"
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
GSM_STATIC size_t CgnsPwrSerialize(void *p, char *obuf, size_t olen)
{
  CgnsPwr_t *obj = (CgnsPwr_t *)p;
  
  if (13 < olen) {
    strncpy(obuf, "AT+CGNSPWR=", olen - 3);
    strncat(obuf, obj->request.mode == 1 ? "1" : "0", olen - 2 - strlen(obuf));
    strncat(obuf, "\r", olen - 1 - strlen(obuf));
  }

  return strlen(obuf);
}

GSM_STATIC size_t CgnsPwrParse(void *p, const char *ibuf, size_t ilen)
{
  CgnsPwr_t *obj            = (CgnsPwr_t *)p;
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

GSM_STATIC void CgnsPwrTimeout(void *p)
{
  CgnsPwr_t *obj = (CgnsPwr_t *)p; 
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CgnsPwrObjectInit(CgnsPwr_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = CgnsPwrSerialize;
  this->atcmd.parse     = CgnsPwrParse;
  this->atcmd.timeout   = CgnsPwrTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void CgnsPwrSetupRequest(CgnsPwr_t *this, uint8_t mode)
{
  this->request.mode = mode;
}

AT_Command_t *CgnsPwrGetAtCommand(CgnsPwr_t *this)
{
  return &this->atcmd;
}

CgnsPwr_response_t CgnsPwrGetResponse(CgnsPwr_t *this)
{
  return this->response;
}

AT_CommandStatus_t CgnsPwrGetResponseStatus(CgnsPwr_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
