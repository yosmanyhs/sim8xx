/**
 * @file btpower.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Common/Env.h"
#include "btsppsend.h"
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
GSM_STATIC size_t BtSppSendSerialize(void *p, char *obuf, size_t olen)
{
  BtSppSend_t *obj = (BtSppSend_t *)p;
  memset(obuf, 0, olen);

  if (BT_SPP_SEND_STATE_COMMAND == obj->state) {
    strncpy(obuf, "AT+BTSPPSEND\r", olen - 1);
  } else if (BT_SPP_SEND_STATE_DATA == obj->state) {
    strncpy(obuf, obj->request.data, olen - 2);
    strncat(obuf, "\x1A", olen - 1 - strlen(obuf));
  } else {
    ;
  }

  return strlen(obuf);
}

GSM_STATIC size_t BtSppSendParse(void *p, const char *ibuf, size_t ilen)
{
  BtSppSend_t *obj          = (BtSppSend_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;
  size_t n                  = AT_CommandStatusParse(ibuf, ilen, &status);

  size_t offset = 0;
  if (BT_SPP_SEND_STATE_COMMAND == obj->state) {
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
  } else if (BT_SPP_SEND_STATE_DATA == obj->state) {
    switch (status) {
    case AT_CMD_SEND_OK:
    case AT_CMD_SEND_FAIL:
    case AT_CMD_ERROR: {
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

GSM_STATIC void BtSppSendTimeout(void *p)
{
  BtSppSend_t *obj     = (BtSppSend_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void BtSppSendObjectInit(BtSppSend_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = BtSppSendSerialize;
  this->atcmd.parse             = BtSppSendParse;
  this->atcmd.timeout           = BtSppSendTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void BtSppSendSetCommandMode(BtSppSend_t *this)
{
  this->state = BT_SPP_SEND_STATE_COMMAND;
}

void BtSppSendSetDataMode(BtSppSend_t *this)
{
  this->state = BT_SPP_SEND_STATE_DATA;
}

void BtSppSendSetupRequest(BtSppSend_t *this, const char *data, size_t length)
{
  this->request.data   = data;
  this->request.length = length;
}

AT_Command_t *BtSppSendGetAtCommand(BtSppSend_t *this)
{
  return &this->atcmd;
}

BtSppSend_response_t BtSppSendGetResponse(BtSppSend_t *this)
{
  return this->response;
}

AT_CommandStatus_t BtSppSendGetResponseStatus(BtSppSend_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
