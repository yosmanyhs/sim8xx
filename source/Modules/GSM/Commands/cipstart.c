/**
 * @file cipstart.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "cipstart.h"
#include "Common/Env.h"
#include "Utils/Utils.h"
#include <stdio.h>
#include <stdlib.h>
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
GSM_STATIC size_t CipstartSerialize(void *p, char *obuf, size_t olen)
{
  Cipstart_t *obj = (Cipstart_t*)p;
  
  memset(obuf, 0, olen);

  if (obj->request.protocol && obj->request.host) {
    strncpy(obuf, "AT+CIPSTART=\"", olen - 1);
    strncat(obuf, obj->request.protocol, olen - 1 - strlen(obuf));
    strncat(obuf, "\",\"", olen - 1 - strlen(obuf));
    strncat(obuf, obj->request.host, olen - 1 - strlen(obuf));
    strncat(obuf, "\",", olen - 1 - strlen(obuf));
    GSM_UtilsItoA(obuf + strlen(obuf), olen - 1 - strlen(obuf), obj->request.port);
    strncat(obuf, "\r", olen - 1 - strlen(obuf));
  }

  return strlen(obuf);
}

GSM_STATIC size_t CipstartParse(void *p, const char *ibuf, size_t ilen)
{
  Cipstart_t *obj = (Cipstart_t*)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t offset = AT_CommandStatusParse(ibuf, ilen, &status);

  if ((AT_CMD_OK == status) || (AT_CMD_ERROR == status)) {
    obj->response.status = status;
  } else {
    const char *tag = "\r\n+CME ERROR";
    size_t tlen     = strlen(tag);
    if ((tlen < ilen) && (0 == strncasecmp(ibuf, tag, tlen))) {
      char *start = strstr(ibuf, "\r\n");
      if (start) {
        char *end = strstr(start + 2, "\r\n");
        if (end) {
          obj->response.status = AT_CMD_ERROR;
          offset               = end + 2 - start;
        } else {
          obj->response.status = AT_CMD_INVALID;
          offset               = 0;
        }
      } else {
        obj->response.status = AT_CMD_INVALID;
        offset               = 0;
      }
    } else {
      offset = 0;
    }
  }

  return offset;
}

GSM_STATIC void CipstartTimeout(void *p)
{
  Cipstart_t *obj = (Cipstart_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CipstartObjectInit(Cipstart_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = CipstartSerialize;
  this->atcmd.parse             = CipstartParse;
  this->atcmd.timeout           = CipstartTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void CipstartSetupRequest(Cipstart_t *this, const char *protocol, const char *host, int32_t port)
{
  this->request.protocol = protocol;
  this->request.host = host;
  this->request.port = port;
}

AT_Command_t *CipstartGetAtCommand(Cipstart_t *this)
{
  return &this->atcmd;
}

Cipstart_Response_t CipstartGetResponse(Cipstart_t *this)
{
  return this->response;
}

AT_CommandStatus_t CipstartGetResponseStatus(Cipstart_t *this)
{
  return this->response.status;
}

bool CipstartIsURC(const char *ibuf, size_t ilen)
{
  bool result = false;

  if (0 == strncasecmp(ibuf, "\r\nALREADY CONNECT\r\n", ilen))
    result = true;
  else if (0 == strncasecmp(ibuf, "\r\nCONNECT OK\r\n", ilen))
    result = true;
  else if (0 == strncasecmp(ibuf, "\r\nSTATE:", ilen < 7 ? ilen : 7))
    result = true;
  else
    result = false;

  return result;
}

size_t CipstartParseURC(CipstartURC_t *urc, const char *ibuf, size_t ilen)
{
  size_t offset = 0;

  memset(urc, 0, sizeof(*urc));

  if (0 == strncasecmp(ibuf, "\r\nALREADY CONNECT\r\n", ilen)) {
    urc->type = CIPSTART_ALREADY_CONNECT;
    offset = 19;
  }
  else if (0 == strncasecmp(ibuf, "\r\nCONNECT OK\r\n", ilen)) {
    urc->type = CIPSTART_CONNECT_OK;
    offset = 14;
  }
  else if (GSM_UtilsBeginsWith(ibuf, "\r\nSTATE:")) {
    urc->type = CIPSTART_CONNECT_FAIL;
    char str[4];
    memset(str, 0, sizeof(str));
    size_t n = GSM_UtilsGetString(ibuf, ilen, str, sizeof(str), ' ', '\r');
    if (0 < n) {
      urc->payload.fail.state = (uint32_t)atoi(str);
      const char *tag = "CONNECT FAIL\r\n";
      size_t tlen = strlen(tag);
      const char *next = ibuf + n + CRLFLENGTH;
      char *end = strstr(next, tag);
      if (end) {
        offset = end + tlen - ibuf;
      } else {
        memset(urc, 0, sizeof(*urc));
        offset = 0;
      }
    }
  }
  else{
    urc->type = CIPSTART_NO_URC;
    offset = 0;
  }

  return offset;
}

/****************************** END OF FILE **********************************/
