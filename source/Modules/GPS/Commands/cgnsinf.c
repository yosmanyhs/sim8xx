/**
 * @file cgnsinf.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "cgnsinf.h"
#include "Utils.h"

#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define TIMEOUT_IN_SEC 10

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
#if !defined(TEST)
static
#endif
    size_t
    CgnsInfSerialize(void *p, char *obuf, size_t length)
{
  CgnsInf_t *obj = (CgnsInf_t *)p;
  size_t n       = 0;
  if (11 < length) {
    strncat(obuf, "AT+CGNSINF", 10);
    n = strlen(obuf);
  }

  return n;
}

#if !defined(TEST)
static
#endif
    size_t
    CgnsInfParse(void *p, const char *ibuf, size_t length)
{
  const char *tag = "\r\n+CGNSINF:";
  if (length < 11)
    return 0;

  if (strncasecmp(ibuf, tag, 11))
    return 0;

  CgnsInf_t *obj            = (CgnsInf_t *)p;
  const char *next = ibuf;
  const char *end = ibuf + length;

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

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CgnsInfObjectInit(CgnsInf_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = CgnsInfSerialize;
  this->atcmd.parse     = CgnsInfParse;
  this->atcmd.timeout   = TIMEOUT_IN_SEC;
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
