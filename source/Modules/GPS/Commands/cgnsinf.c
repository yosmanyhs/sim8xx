/**
 * @file cgnsinf.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "cgnsinf.h"
#include "Common/Env.h"
#include "Utils/Utils.h"

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
GSM_STATIC size_t CgnsInfSerialize(void *p, char *obuf, size_t olen)
{
  (void)p;
  memset(obuf, 0, olen);

  if (11 < olen) {
    strncpy(obuf, "AT+CGNSINF\r", olen - 1);
  }

  return strlen(obuf);
}

GSM_STATIC size_t CgnsInfParse(void *p, const char *ibuf, size_t ilen)
{
  const char *tag = "\r\n+CGNSINF:";
  if (ilen < 11)
    return 0;

  if (strncasecmp(ibuf, tag, 11))
    return 0;

  CgnsInf_t *obj            = (CgnsInf_t *)p;
  const char *next = ibuf;
  const char *end = ibuf + ilen;

  size_t offset = 0;
  size_t n = GSM_UtilsGetInt(next, end - next, &obj->response.runStatus, ' ', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetInt(next, end - next, &obj->response.fixStatus, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetString(next, end - next, obj->response.date, sizeof(obj->response.date), ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetDouble(next, end - next, &obj->response.latitude, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetDouble(next, end - next, &obj->response.longitude, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetDouble(next, end - next, &obj->response.altitude, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetDouble(next, end - next, &obj->response.speed, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetDouble(next, end - next, &obj->response.course, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetInt(next, end - next, &obj->response.fixMode, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsSkipReserved(next, end - next, ',', 1);
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetDouble(next, end - next, &obj->response.hdop, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetDouble(next, end - next, &obj->response.pdop, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetDouble(next, end - next, &obj->response.vdop, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsSkipReserved(next, end - next, ',', 1);
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetInt(next, end - next, &obj->response.gpsSatInView, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetInt(next, end - next, &obj->response.gnssSatInUse, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetInt(next, end - next, &obj->response.gnssSatInView, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsSkipReserved(next, end - next, ',', 1);
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetInt(next, end - next, &obj->response.cnomax, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetDouble(next, end - next, &obj->response.hpa, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  n = GSM_UtilsGetDouble(next, end - next, &obj->response.vpa, ',', '\r');
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
  n = AT_CommandStatusParse(next, end - next, &status);

  if (n && (AT_CMD_OK == status)) {
    obj->response.status = status;
    offset += n;
  } else {
    memset(&obj->response, 0, sizeof(obj->response));
    return 0;
  }

  return offset;
}

GSM_STATIC void CgnsInfTimeout(void *p)
{
  CgnsInf_t *obj = (CgnsInf_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CgnsInfObjectInit(CgnsInf_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = CgnsInfSerialize;
  this->atcmd.parse     = CgnsInfParse;
  this->atcmd.timeout   = CgnsInfTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

AT_Command_t *CgnsInfGetAtCommand(CgnsInf_t *this)
{
  return &this->atcmd;
}

CgnsInf_response_t CgnsInfGetResponse(CgnsInf_t *this)
{
  return this->response;
}

AT_CommandStatus_t CgnsInfGetResponseStatus(CgnsInf_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
