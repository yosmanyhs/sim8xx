/**
 * @file csq.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "csq.h"
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
GSM_STATIC size_t CsqSerialize(void *p, char *obuf, size_t olen)
{
  (void)p;
  
  memset(obuf, 0, olen);

  strncpy(obuf, "AT+CSQ\r", olen - 1);

  return strlen(obuf);
}

GSM_STATIC size_t CsqParse(void *p, const char *ibuf, size_t ilen)
{
  Csq_t *obj = (Csq_t*)p;
  size_t offset = 0;

  if ((12 < ilen) && (0 == strncasecmp(ibuf, "\r\n+CME ERROR", 12))) {
    char *start          = strstr(ibuf, "\r\n");
    char *end            = strstr(start + 2, "\r\n");
    obj->response.status = AT_CMD_ERROR;
    offset               = end + 2 - start;
  } 
  else if ((6 < ilen) && (0 == strncasecmp(ibuf, "\r\n+CSQ", 6))) {
    const char *next = ibuf;
    const char *end  = ibuf + ilen;

    size_t n = GSM_UtilsGetInt(next, end - next, &obj->response.rssi, ' ', ',');
    if (n) {
      offset += n;
      next += n;
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }

    n = GSM_UtilsGetInt(next, end - next, &obj->response.ber, ',', '\r');
    if (n) {
      offset += n;
      next += n;
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }

    if (strncasecmp(CRLF, next, CRLFLENGTH)) {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    } else {
      offset += CRLFLENGTH;
      next += CRLFLENGTH;
    }

    AT_CommandStatus_t status = AT_CMD_INVALID;
    n                         = AT_CommandStatusParse(next, end - next, &status);

    switch (status) {
    case AT_CMD_OK:
    case AT_CMD_ERROR:
      obj->response.status = status;
      offset += n;
      break;
    default:
      obj->response.status = AT_CMD_INVALID;
      offset               = 0;
      break;
    }
  }

  return offset;
}

GSM_STATIC void CsqTimeout(void *p)
{
  Csq_t *obj = (Csq_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CsqObjectInit(Csq_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = CsqSerialize;
  this->atcmd.parse             = CsqParse;
  this->atcmd.timeout           = CsqTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void CsqSetupRequest(Csq_t *this)
{
  (void)this;
}

AT_Command_t *CsqGetAtCommand(Csq_t *this)
{
  return &this->atcmd;
}

Csq_Response_t CsqGetResponse(Csq_t *this)
{
  return this->response;
}

AT_CommandStatus_t CsqGetResponseStatus(Csq_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
