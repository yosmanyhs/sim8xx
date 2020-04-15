/**
 * @file sapbr.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "sapbr.h"
#include "Common/Env.h"
#include "Utils/Utils.h"
#include <stdio.h>
#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define TIMEOUT_IN_MSEC 100000

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static const char *CmdTypeStr[] = {
    [CMD_TYPE_CLOSE] = "0",
    [CMD_TYPE_OPEN]  = "1",
    [CMD_TYPE_QUERY] = "2",
    [CMD_TYPE_SET]   = "3",
    [CMD_TYPE_GET]   = "4",
};

static const char *ParamTagStr[] = {
  [PARAM_TAG_CONNTYPE] = "CONTYPE",
  [PARAM_TAG_APN] = "APN",
  [PARAM_TAG_USER] = "USER",
  [PARAM_TAG_PWD] = "PWD",
  [PARAM_TAG_PHONENUM] = "PHONENUM",
  [PARAM_TAG_RATE] = "RATE",
};

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
GSM_STATIC size_t SapbrSerialize(void *p, char *obuf, size_t olen)
{
  Sapbr_t *obj = (Sapbr_t*)p;
  
  memset(obuf, 0, olen);

  char id[5];
  memset(id, 0, sizeof(id));
  GSM_UtilsItoA(id, sizeof(id), obj->request.id);

  strncpy(obuf, "AT+SAPBR=", olen - 1);
  strncat(obuf, CmdTypeStr[obj->request.cmd], olen - 1 - strlen(obuf));
  strncat(obuf, ",", olen - 1 - strlen(obuf));
  strncat(obuf, id, olen - 1 - strlen(obuf));

  if (CMD_TYPE_SET == obj->request.cmd) {
    strncat(obuf, ",\"", olen - 1 - strlen(obuf));
    strncat(obuf, ParamTagStr[obj->request.tag], olen - 1 - strlen(obuf));
    strncat(obuf, "\",\"", olen - 1 - strlen(obuf));
    strncat(obuf, obj->request.value, olen - 1 - strlen(obuf));
    strncat(obuf, "\"\r", olen - 1 - strlen(obuf));
  } else if (CMD_TYPE_GET == obj->request.cmd) {
    strncat(obuf, ",\"", olen - 1 - strlen(obuf));
    strncat(obuf, ParamTagStr[obj->request.tag], olen - 1 - strlen(obuf));
    strncat(obuf, "\"\r", olen - 1 - strlen(obuf));
  } else {
    strncat(obuf, "\r", olen - 1 - strlen(obuf));
  }

  return strlen(obuf);
}

GSM_STATIC size_t SapbrParseQuery(Sapbr_t *obj, const char ibuf[], size_t ilen)
{
  memset(&obj->response, 0, sizeof(obj->response));

  size_t offset = 0;

  if (GSM_UtilsBeginsWith(ibuf, "\r\n+SAPBR")) {
    const char *next = ibuf;
    const char *end  = ibuf + ilen;
    int32_t *pid = &obj->response.payload.query.id;
    int32_t *pstatus = &obj->response.payload.query.status;
    char *pipaddr = obj->response.payload.query.ipaddr;
    size_t ipaddrlen = sizeof(obj->response.payload.query.ipaddr);
    
    size_t n = GSM_UtilsGetInt(next, end - next, pid, ':', ',');
    if (n) {
      offset += n;
      next += n;
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }
    
    n = GSM_UtilsGetInt(next, end - next, pstatus, ',', ',');
    if (n) {
      offset += n;
      next += n;
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }

    n = GSM_UtilsGetString(next, end - next, pipaddr, ipaddrlen, '\"', '\"');
    if (n) {
      offset += n;
      next += n;
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }

    // Skip '"'
    offset += 1;
    next += 1; 

    if (strncasecmp(CRLF, next, CRLFLENGTH)) {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    } else {
      offset += CRLFLENGTH;
      next += CRLFLENGTH;
    }

    AT_CommandStatus_t status = AT_CMD_INVALID;
    n = AT_CommandStatusParse(next, end - next, &status);

    if (AT_CMD_OK == status) {
      obj->response.status = status;
      offset += n;
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }
  }
  return offset;
}

GSM_STATIC size_t SapbrParseGet(Sapbr_t *obj, const char ibuf[], size_t ilen)
{
  memset(&obj->response, 0, sizeof(obj->response));

  size_t offset = 0;

  if (GSM_UtilsBeginsWith(ibuf, "\r\n+SAPBR")) {
    const char *next = ibuf;
    const char *end  = ibuf + ilen;
    char *tag = obj->response.payload.get.tag;
    size_t tlen = sizeof(obj->response.payload.get.tag);
    char *value = obj->response.payload.get.value;
    size_t vlen = sizeof(obj->response.payload.get.value);
    
    size_t n = GSM_UtilsGetString(next, end - next, tag, tlen, '"', '"');
    if (n) {
      offset += n;
      next += n;
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }

    // Skip '"'
    offset += 1;
    next += 1;

    n = GSM_UtilsGetString(next, end - next, value, vlen, '"', '"');
    if (n) {
      offset += n;
      next += n;
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }

    // Skip '"'
    offset += 1;
    next += 1; 

    if (strncasecmp(CRLF, next, CRLFLENGTH)) {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    } else {
      offset += CRLFLENGTH;
      next += CRLFLENGTH;
    }

    AT_CommandStatus_t status = AT_CMD_INVALID;
    n = AT_CommandStatusParse(next, end - next, &status);

    if (AT_CMD_OK == status) {
      obj->response.status = status;
      offset += n;
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }
  }
  return offset;
}

GSM_STATIC size_t SapbrParse(void *p, const char *ibuf, size_t ilen)
{
  Sapbr_t *obj = (Sapbr_t*)p;
  size_t offset = 0;

  switch (obj->request.cmd) {
  case CMD_TYPE_CLOSE:
  case CMD_TYPE_OPEN:
  case CMD_TYPE_SET: {
    AT_CommandStatus_t status = AT_CMD_INVALID;
    size_t n                  = AT_CommandStatusParse(ibuf, ilen, &status);
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
    break;
  }
  case CMD_TYPE_QUERY: {
    offset = SapbrParseQuery(obj, ibuf, ilen);
    break;
  }
  case CMD_TYPE_GET: {
    offset = SapbrParseGet(obj, ibuf, ilen);
    break;
  }
  default: {
    obj->response.status = AT_CMD_INVALID;
    offset               = 0;
    break;
  }
  }

  return offset;
}

GSM_STATIC void SapbrTimeout(void *p)
{
  Sapbr_t *obj = (Sapbr_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void SapbrObjectInit(Sapbr_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = SapbrSerialize;
  this->atcmd.parse             = SapbrParse;
  this->atcmd.timeout           = SapbrTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void SapbrSetupRequestSetConnTypeGprs(Sapbr_t *this, int32_t id)
{
  this->request.cmd = CMD_TYPE_SET;
  this->request.id = id;
  this->request.tag = PARAM_TAG_CONNTYPE;
  this->request.value = "GPRS";
}

void SapbrSetupRequestSetApn(Sapbr_t *this, int32_t id, const char *apn)
{
  this->request.cmd = CMD_TYPE_SET;
  this->request.id = id;
  this->request.tag = PARAM_TAG_APN;
  this->request.value = apn;
}

void SapbrSetupRequestOpenContext(Sapbr_t *this, int32_t id)
{
  this->request.cmd = CMD_TYPE_OPEN;
  this->request.id = id;
}

void SapbrSetupRequestCloseContext(Sapbr_t *this, int32_t id)
{
  this->request.cmd = CMD_TYPE_CLOSE;
  this->request.id = id;
}

AT_Command_t *SapbrGetAtCommand(Sapbr_t *this)
{
  return &this->atcmd;
}

Sapbr_Response_t SapbrGetResponse(Sapbr_t *this)
{
  return this->response;
}

AT_CommandStatus_t SapbrGetResponseStatus(Sapbr_t *this)
{
  return this->response.status;
}

bool SapbrIsURC(const char *ibuf, size_t ilen)
{
  (void)ilen;
  bool result = false;

  if (GSM_UtilsBeginsWith(ibuf, "\r\n+SAPBR"))
    result = true;

  return result;
}

size_t SapbrParseURC(SapbrURC_t *urc, const char *ibuf, size_t ilen)
{
  memset(urc, 0, sizeof(*urc));
  
  size_t offset = 0;
  if (GSM_UtilsBeginsWith(ibuf, "\r\n+SAPBR")) {
    const char *next = ibuf;
    const char *end  = ibuf + ilen;
    size_t n = GSM_UtilsGetInt(ibuf, ilen, &urc->payload.deact.id, ' ', ':');
    if (n) {
      offset += n;
      next += n;
    } else {
      memset(urc, 0, sizeof(*urc));
      return 0;
    }

    const char *tag = " DEACT\r\n";
    size_t tlen = strlen(tag);
    end = strstr(ibuf, tag);
    if (end) {
      offset = end + tlen - ibuf;
      urc->type = SAPBR_URC_DEACT;
    } else {
      memset(urc, 0, sizeof(*urc));
      return 0;
    }
  }

  return offset;
}

/****************************** END OF FILE **********************************/
