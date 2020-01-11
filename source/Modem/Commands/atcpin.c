/**
 * @file atcpin.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "atcpin.h"

#include "Common/Env.h"
#include "Utils.h"

#include <stdio.h>
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
GSM_STATIC size_t AtCpinSerialize(void *p, char *obuf, size_t olen)
{
  memset(obuf, 0, olen);

  AtCpin_t *obj = (AtCpin_t *)p;

  if (obj->request.pin) {
    strncpy(obuf, "AT+CPIN=", olen - 1);
    strncat(obuf, obj->request.pin, olen - 1 - strlen(obuf));
  }

  if (obj->request.pin && obj->request.pin2) {
    strncat(obuf, ",", olen - 1 - strlen(obuf));
    strncat(obuf, obj->request.pin2, olen - 1 - strlen(obuf));
  }

  if (obj->request.pin) {
    strncat(obuf, "\r", olen - 1 - strlen(obuf));
  }

  return strlen(obuf);
}

GSM_STATIC size_t AtCpinParse(void *p, const char *ibuf, size_t ilen)
{
  AtCpin_t *obj             = (AtCpin_t *)p;
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

GSM_STATIC void AtCpinTimeout(void *p)
{
  AtCpin_t *obj        = (AtCpin_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

GSM_STATIC AtCpinCode_t AtCpin_convertStringToCode(const char ibuf[], size_t ilen)
{
  AtCpinCode_t code = ATCPIN_INVALID_CODE;

  if ((5 == ilen) && (0 == strncasecmp(ibuf, "READY", ilen))) {
    code = ATCPIN_READY;
  } else if ((7 == ilen) && (0 == strncasecmp(ibuf, "SIM PIN", ilen))) {
    code = ATCPIN_SIM_PIN;
  } else if ((7 == ilen) && (0 == strncasecmp(ibuf, "SIM PUK", ilen))) {
    code = ATCPIN_SIM_PUK;
  } else if ((10 == ilen) && (0 == strncasecmp(ibuf, "PH_SIM PIN", ilen))) {
    code = ATCPIN_PH_SIM_PIN;
  } else if ((10 == ilen) && (0 == strncasecmp(ibuf, "PH_SIM PUK", ilen))) {
    code = ATCPIN_PH_SIM_PUK;
  } else if ((8 == ilen) && (0 == strncasecmp(ibuf, "SIM PIN2", ilen))) {
    code = ATCPIN_SIM_PIN2;
  } else if ((8 == ilen) && (0 == strncasecmp(ibuf, "SIM PUK2", ilen))) {
    code = ATCPIN_SIM_PUK2;
  } else {
    code = ATCPIN_INVALID_CODE;
  }

  return code;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void AtCpinObjectInit(AtCpin_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = AtCpinSerialize;
  this->atcmd.parse             = AtCpinParse;
  this->atcmd.timeout           = AtCpinTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void AtCpinSetupRequest(AtCpin_t *this, const char *pin, const char *pin2)
{
  this->request.pin  = pin ? pin : NULL;
  this->request.pin2 = pin2 ? pin2 : NULL;
}

AT_Command_t *AtCpinGetAtCommand(AtCpin_t *this)
{
  return &this->atcmd;
}

AtCpin_Response_t AtCpinGetResponse(AtCpin_t *this)
{
  return this->response;
}

AT_CommandStatus_t AtCpinGetResponseStatus(AtCpin_t *this)
{
  return this->response.status;
}

bool AtCpinIsURC(const char *ibuf, size_t ilen)
{
  const char *tag = "\r\n+CPIN";
  size_t tlen     = strlen(tag);

  bool result = false;
  if (tlen < ilen) {
    result = (0 == strncasecmp(ibuf, tag, tlen));
  }

  return result;
}

size_t AtCpinParseURC(AtCpinURC_t *urc, const char *ibuf, size_t ilen)
{
  if (!AtCpinIsURC(ibuf, ilen))
    return 0;

  size_t offset = 0;
  memset(urc, 0, sizeof(*urc));

  const char *next = ibuf;
  const char *end  = ibuf + ilen;

  char str[32] = {0};
  size_t n     = GSM_UtilsGetString(next, end - next, str, sizeof(str), ' ', '\r');
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

  urc->type              = ATCPIN_INFO;
  urc->payload.info.code = AtCpin_convertStringToCode(str, strlen(str));

  return offset;
}

/****************************** END OF FILE **********************************/
