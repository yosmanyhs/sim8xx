/**
 * @file cipsend.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "cipsend.h"
#include "Common/Env.h"
#include <stdio.h>
#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define TIMEOUT_IN_MSEC 645000

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
GSM_STATIC size_t CipsendSerialize(void *p, char *obuf, size_t olen)
{
  Cipsend_t *obj = (Cipsend_t*)p;
  
  memset(obuf, 0, olen);

  switch(obj->mode) {
    case CIPSEND_COMMAND: {
      strncpy(obuf, "AT+CIPSEND\r", olen);
      break;
    }
    case CIPSEND_DATA: {
      strncpy(obuf, obj->request.data, olen - 2);
      strncat(obuf, "\x1A", olen - 1 - strlen(obuf));
      break;
    }
    default: {
      break;
    }
  }

  return strlen(obuf);
}

GSM_STATIC size_t CipsendParse(void *p, const char *ibuf, size_t ilen)
{
  Cipsend_t *obj = (Cipsend_t*)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;
  size_t n                  = AT_CommandStatusParse(ibuf, ilen, &status);

  size_t offset = 0;
  if (CIPSEND_COMMAND == obj->mode) {
    switch (status) {
    case AT_CMD_WAIT_FOR_USER_DATA: {
      obj->response.status = status;
      offset               = n;
      break;
    }
    default: {
      obj->response.status = AT_CMD_INVALID;
      offset               = 0;
      break;
    }
    }
  } else if (CIPSEND_DATA == obj->mode) {
    switch (status) {
    case AT_CMD_SEND_OK:
    case AT_CMD_SEND_FAIL:{
      obj->response.status = status;
      offset               = n;
      break;
    }
    default: {
      obj->response.status = AT_CMD_INVALID;
      offset               = 0;
      break;
    }
    }
  } else {
    ;
  }


  return offset;
}

GSM_STATIC void CipsendTimeout(void *p)
{
  Cipsend_t *obj = (Cipsend_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CipsendObjectInit(Cipsend_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = CipsendSerialize;
  this->atcmd.parse             = CipsendParse;
  this->atcmd.timeout           = CipsendTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
  this->mode                    = CIPSEND_COMMAND;
}

void CipsendSetCommandMode(Cipsend_t *this)
{
  this->mode = CIPSEND_COMMAND;
}

void CipsendSetDataMode(Cipsend_t *this)
{
  this->mode = CIPSEND_DATA;
}

void CipsendSetupRequest(Cipsend_t *this, const char *data, size_t length)
{
  this->request.data = data;
  this->request.length = length;
}

AT_Command_t *CipsendGetAtCommand(Cipsend_t *this)
{
  return &this->atcmd;
}

Cipsend_Response_t CipsendGetResponse(Cipsend_t *this)
{
  return this->response;
}

AT_CommandStatus_t CipsendGetResponseStatus(Cipsend_t *this)
{
  return this->response.status;
}

bool CipsendIsURC(const char *ibuf, size_t ilen)
{
  bool result = false;

  if (0 == strncasecmp(ibuf, "\r\nSEND OK\r\n", ilen))
    result = true;
  else if (0 == strncasecmp(ibuf, "\r\nSEND FAIL\r\n", ilen))
    result = true;
  else
    result = false;

  return result;
}

size_t CipsendParseURC(CipsendURC_t *urc, const char *ibuf, size_t ilen)
{
  size_t offset = 0;

  memset(urc, 0, sizeof(*urc));

  if (0 == strncasecmp(ibuf, "\r\nSEND OK\r\n", ilen)) {
    urc->type = CIPSEND_SEND_OK;
    offset = 11;
  }
  else if (0 == strncasecmp(ibuf, "\r\nSEND FAIL\r\n", ilen)) {
    urc->type = CIPSEND_SEND_FAIL;
    offset = 13;
  }
  else {
    urc->type = CIPSEND_NO_URC;
    offset = 0;
  }

  return offset;
}

/****************************** END OF FILE **********************************/
