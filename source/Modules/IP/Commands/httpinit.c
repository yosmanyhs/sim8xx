/**
 * @file httpinit.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "httpinit.h"
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
GSM_STATIC size_t HttpinitSerialize(void *p, char *obuf, size_t olen)
{
  (void)p;
  
  memset(obuf, 0, olen);

  strncpy(obuf, "AT+HTTPINIT\r", olen - 1);

  return strlen(obuf);
}

GSM_STATIC size_t HttpinitParse(void *p, const char *ibuf, size_t ilen)
{
  Httpinit_t *obj           = (Httpinit_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;
  size_t offset             = 0;
  size_t n                  = AT_CommandStatusParse(ibuf, ilen, &status);

  if (AT_CMD_OK == status) {
    obj->response.status = status;
    offset = n;
  } else {
    const char *tag = "\r\n+CME ERROR";
    size_t tlen     = strlen(tag);
    if ((tlen < ilen) && (0 == strncasecmp(ibuf, tag, tlen))) {
      char *start          = strstr(ibuf, "\r\n");
      char *end            = strstr(start + 2, "\r\n");
      obj->response.status = AT_CMD_ERROR;
      offset               = end + 2 - start;
    } else {
      offset = 0;
    }
  }

  return offset;
}

GSM_STATIC void HttpinitTimeout(void *p)
{
  Httpinit_t *obj = (Httpinit_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void HttpinitObjectInit(Httpinit_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = HttpinitSerialize;
  this->atcmd.parse             = HttpinitParse;
  this->atcmd.timeout           = HttpinitTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void HttpinitSetupRequest(Httpinit_t *this)
{
  (void)this;
}

AT_Command_t *HttpinitGetAtCommand(Httpinit_t *this)
{
  return &this->atcmd;
}

Httpinit_Response_t HttpinitGetResponse(Httpinit_t *this)
{
  return this->response;
}

AT_CommandStatus_t HttpinitGetResponseStatus(Httpinit_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
