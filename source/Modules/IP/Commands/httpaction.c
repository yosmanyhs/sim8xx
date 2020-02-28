/**
 * @file httpaction.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "httpaction.h"
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
GSM_STATIC size_t HttpactionSerialize(void *p, char *obuf, size_t olen)
{
  Httpaction_t *obj = (Httpaction_t*)p;
  
  memset(obuf, 0, olen);

  strncpy(obuf, "AT+HTTPACTION=", olen - 1);
  GSM_UtilsItoA(obuf + strlen(obuf), olen - 1 - strlen(obuf), (int32_t)obj->request.method);
  strncat(obuf, "\r", olen - 1 - strlen(obuf));

  return strlen(obuf);
}

GSM_STATIC size_t HttpactionParse(void *p, const char *ibuf, size_t ilen)
{
  Httpaction_t *obj = (Httpaction_t*)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t offset = 0;
  size_t n = AT_CommandStatusParse(ibuf, ilen, &status);

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

GSM_STATIC void HttpactionTimeout(void *p)
{
  Httpaction_t *obj = (Httpaction_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void HttpactionObjectInit(Httpaction_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = HttpactionSerialize;
  this->atcmd.parse             = HttpactionParse;
  this->atcmd.timeout           = HttpactionTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void HttpactionSetupRequest(Httpaction_t *this, HttpactionMethod_t method)
{
  this->request.method = method;
}

AT_Command_t *HttpactionGetAtCommand(Httpaction_t *this)
{
  return &this->atcmd;
}

Httpaction_Response_t HttpactionGetResponse(Httpaction_t *this)
{
  return this->response;
}

AT_CommandStatus_t HttpactionGetResponseStatus(Httpaction_t *this)
{
  return this->response.status;
}

bool HttpactionIsURC(const char *ibuf, size_t ilen)
{
  return GSM_UtilsBeginsWith(ibuf, "\r\n+HTTPACTION");
}

size_t HttpactionParseURC(HttpactionURC_t *urc, const char *ibuf, size_t ilen)
{
  size_t offset = 0;
  memset(urc, 0, sizeof(*urc));

  const char *next = ibuf;
  const char *end  = ibuf + ilen;

  size_t n = GSM_UtilsGetInt(next, end - next, (int32_t*)&urc->method, ' ', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  n = GSM_UtilsGetInt(next, end - next, (int32_t*)&urc->httpStatus, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  n = GSM_UtilsGetInt(next, end - next, (int32_t*)&urc->dataLength, ',', '\r');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  if (0 == strncmp(next, "\r\n", 2)) {
    offset += 2;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  return offset;
}

/****************************** END OF FILE **********************************/
