/**
 * @file atcfun.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "cfun.h"

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
GSM_STATIC size_t CfunSerialize(void *p, char *obuf, size_t olen)
{
  memset(obuf, 0, olen);

  Cfun_t *obj = (Cfun_t *)p;

  strncpy(obuf, "AT+CFUN=", olen - 1);

  size_t n = strlen(obuf);
  GSM_UtilsItoA(obuf + n, olen - 1 - n, obj->request.fun);

  strncat(obuf, ",", olen - 1 - strlen(obuf));

  n = strlen(obuf);
  GSM_UtilsItoA(obuf + n, olen - 1 - n, obj->request.rst);

  strncat(obuf, "\r", olen - 1 - strlen(obuf));

  return strlen(obuf);
}

GSM_STATIC size_t CfunParse(void *p, const char *ibuf, size_t ilen)
{
  Cfun_t *obj             = (Cfun_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t offset = AT_CommandStatusParse(ibuf, ilen, &status);

  if (offset && (AT_CMD_OK == status)) {
    obj->response.status = status;
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

GSM_STATIC void CfunTimeout(void *p)
{
  Cfun_t *obj          = (Cfun_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CfunObjectInit(Cfun_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = CfunSerialize;
  this->atcmd.parse             = CfunParse;
  this->atcmd.timeout           = CfunTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void CfunSetupRequest(Cfun_t *this, int32_t fun, int32_t rst)
{
  this->request.fun = fun;
  this->request.rst = rst;
}

AT_Command_t *CfunGetAtCommand(Cfun_t *this)
{
  return &this->atcmd;
}

Cfun_Response_t CfunGetResponse(Cfun_t *this)
{
  return this->response;
}

AT_CommandStatus_t CfunGetResponseStatus(Cfun_t *this)
{
  return this->response.status;
}

bool CfunIsURC(const char *ibuf, size_t ilen)
{
  const char *tag = "\r\n+CFUN";
  size_t tlen     = strlen(tag);

  bool result = false;
  if (tlen < ilen) {
    result = (0 == strncasecmp(ibuf, tag, tlen));
  }

  return result;
}

size_t CfunParseURC(CfunURC_t *urc, const char *ibuf, size_t ilen)
{
  if (!CfunIsURC(ibuf, ilen))
    return 0;

  size_t offset = 0;
  memset(urc, 0, sizeof(*urc));

  const char *next = ibuf;
  const char *end  = ibuf + ilen;

  int32_t level = 0;
  size_t n  = GSM_UtilsGetInt(next, end - next, &level, ' ', '\r');
  if (n) {
    offset += n;
    next += n;
    urc->type = CFUN_INFO;
    if (1 == level)
      urc->payload.info.code = CFUN_LEVEL_MINIMUM;
    else if (2 == level)
      urc->payload.info.code = CFUN_LEVEL_FULL;
    else if (4 == level)
      urc->payload.info.code = CFUN_LEVEL_DISABLE_PHONE;
    else
      urc->payload.info.code = CFUN_LEVEL_INVALID;
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
