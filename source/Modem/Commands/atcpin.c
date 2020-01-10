/**
 * @file atcpin.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Common/Env.h"
#include "Utils.h"
#include "atcpin.h"
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
GSM_STATIC size_t AtCpinSerialize(void *p, char *obuf, size_t length)
{
  memset(obuf, 0, length);

  AtCpin_t *obj = (AtCpin_t *)p;

  if ('\0' == obj->request.pin[0])
    return 0;

  if (14 < length) {
    strncpy(obuf, "AT+CPIN=", length);
    strncat(obuf, obj->request.pin, length - strlen(obuf));
  }

  if (('\0' != obj->request.pin2[0]) && (19 < length)) {
    strncat(obuf, ",", length - strlen(obuf));
    strncat(obuf, obj->request.pin2, length - strlen(obuf));
  }

  if (strlen(obuf) < (length - 2))
    obuf[strlen(obuf)] = '\r';

  return strlen(obuf);
}

GSM_STATIC size_t AtCpinParse(void *p, const char *ibuf, size_t length)
{
  AtCpin_t *obj             = (AtCpin_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t offset = AT_CommandStatusParse(ibuf, length, &status);

  if (0 == offset) {
    // TODO Add CME error handling.
  }

  if (offset && (AT_CMD_OK == status)) {
    obj->response.status = status;
  }

  return offset;
}

GSM_STATIC void AtCpinTimeout(void *p)
{
  AtCpin_t *obj        = (AtCpin_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

GSM_STATIC AtCpinCode_t AtCpin_convertResultStringToCode(const char ibuf[], size_t length)
{
  AtCpinCode_t code = ATCPIN_INVALID_CODE;

  if ((5 <= length) && (0 == strncasecmp(ibuf, "READY", length))) {
    code = ATCPIN_READY;
  } else if ((7 <= length) && (0 == strncasecmp(ibuf, "SIM PIN", length))) {
    code = ATCPIN_SIM_PIN;
  } else if ((7 <= length) && (0 == strncasecmp(ibuf, "SIM PUK", length))) {
    code = ATCPIN_SIM_PUK;
  } else if ((10 <= length) && (0 == strncasecmp(ibuf, "PH_SIM PIN", length))) {
    code = ATCPIN_PH_SIM_PIN;
  } else if ((10 <= length) && (0 == strncasecmp(ibuf, "PH_SIM PUK", length))) {
    code = ATCPIN_PH_SIM_PUK;
  } else if ((8 <= length) && (0 == strncasecmp(ibuf, "SIM PIN2", length))) {
    code = ATCPIN_SIM_PIN2;
  } else if ((8 <= length) && (0 == strncasecmp(ibuf, "SIM PUK2", length))) {
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
  memset(this->request.pin, 0, 4);
  memset(this->request.pin2, 0, 4);

  if (pin)
    memcpy(this->request.pin, pin, 4);

  if (pin2)
    memcpy(this->request.pin2, pin2, 4);
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

bool AtCpinIsURC(const char *ibuf, size_t length)
{
  const char *tag = "\r\n+CPIN:";
  size_t n        = strlen(tag);
  if (length < n)
    n = length;

  return (0 == strncasecmp(ibuf, tag, n));
}

size_t AtCpinParseURC(AtCpinURC_t *urc, const char *ibuf, size_t length)
{
  size_t offset = 0;
  memset(urc, 0, sizeof(*urc));

  const char *next = ibuf;
  const char *end  = ibuf + length;

  char codestr[32] = {0};
  size_t n         = GSM_UtilsGetString(next, end - next, codestr, sizeof(codestr), ' ', '\r');
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

  urc->type = ATCPIN_INFO;
  urc->payload.info.code = AtCpin_convertResultStringToCode(codestr, strlen(codestr));

  return offset;
}

/****************************** END OF FILE **********************************/
