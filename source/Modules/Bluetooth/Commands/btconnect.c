/**
 * @file btconnect.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "btconnect.h"
#include "Common/Env.h"
#include "Utils/Utils.h"

#include <string.h>
#include <stdio.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define TIMEOUT_IN_SEC 5

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
GSM_STATIC bool BtConnect_isBtConnecting(const char *ibuf, size_t length)
{   
    const char *tag = "\r\n+BTCONNECTING:";
    return (0 == strncasecmp(ibuf, tag, length));
}

GSM_STATIC size_t BtConnect_parseBtConnecting(BtConnect_ConnectingURC_t *urc, const char *ibuf, size_t length)
{
    size_t offset = 0;
    memset(urc, 0, sizeof(*urc));

    if (!BtConnect_isBtConnecting(ibuf, length))
        return 0;

    const char *next = ibuf;
    const char *end = ibuf + length;

    size_t n = GSM_UtilsGetString(next, end - next, urc->address, sizeof(urc->address), '"', '"');
    if (n) {
        offset += n + 1;
        next += n + 1;
    } else {
        memset(urc, 0, sizeof(*urc));
        return 0;
    }
    
    n = GSM_UtilsGetString(next, end - next, urc->profile, sizeof(urc->profile), '"', '"');
    if (n) {
        offset += n;
        next += n + 1;
    } else {
        memset(urc, 0, sizeof(*urc));
        return 0;
    }

    if (0 == strncmp(next, "\r\n", 2)) {
        offset += 2 + 1;
    } else {
        memset(urc, 0, sizeof(*urc));
        return 0;
    }
    
    return offset;
}

GSM_STATIC bool BtConnect_isBtConnect(const char *ibuf, size_t length)
{   
    const char *tag = "\r\n+BTCONNECT:";
    return (0 == strncasecmp(ibuf, tag, length));
}

GSM_STATIC size_t BtConnect_parseBtConnect(BtConnect_Result_t *urc, const char *ibuf, size_t length)
{
    size_t offset = 0;
    memset(urc, 0, sizeof(*urc));

    if (!BtConnect_isBtConnect(ibuf, length))
        return 0;

    const char *next = ibuf;
    const char *end = ibuf + length;

    size_t n = GSM_UtilsGetInt(next, end - next, &urc->id, ' ', ',');
    if (n) {
        offset += n + 1;
        next += n + 1;
    } else {
        memset(urc, 0, sizeof(*urc));
        return 0;
    }

    n = GSM_UtilsGetString(next, end - next, urc->name, sizeof(urc->name), '"', '"');
    if (n) {
        offset += n + 1;
        next += n + 1;
    } else {
        memset(urc, 0, sizeof(*urc));
        return 0;
    }

    n = GSM_UtilsGetString(next, end - next, urc->address, sizeof(urc->address), ',', ',');
    if (n) {
        offset += n + 1;
        next += n + 1;
    } else {
        memset(urc, 0, sizeof(*urc));
        return 0;
    }
    
    n = GSM_UtilsGetString(next, end - next, urc->profile, sizeof(urc->profile), '"', '"');
    if (n) {
        offset += n;
        next += n + 1;
    } else {
        memset(urc, 0, sizeof(*urc));
        return 0;
    }

    if (0 == strncmp(next, "\r\n", 2)) {
        offset += 2 + 1;
    } else {
        memset(urc, 0, sizeof(*urc));
        return 0;
    }
    
    return offset;
}

GSM_STATIC size_t BtConnectSerialize(void *p, char *obuf, size_t length)
{
  BtConnect_t *obj = (BtConnect_t *)p;
  size_t n      = 0;

  if (18 < length) {
    strncpy(obuf, "AT+BTCONNECT=", length);
    n = strlen(obuf);
    n += GSM_UtilsItoA(obuf + n, length - n, obj->request.deviceId);
    strncat(obuf, ",", length - n);
    ++n;
    n += GSM_UtilsItoA(obuf + n, length - n, obj->request.profileId);
  }

  return n;
}

GSM_STATIC size_t BtConnectParse(void *p, const char *ibuf, size_t length)
{
  BtConnect_t *obj          = (BtConnect_t *)p;

  size_t n = 0;
  if (0 == strncasecmp(ibuf, "\r\n+BTCONNECT: 0\r\n", 17)) {
    n = 17;
    obj->response.status = AT_CMD_ERROR;
  } else {
    n = BtConnect_parseBtConnect(&obj->response.result, ibuf, length);
    obj->response.status = (0 < n) ? AT_CMD_OK : AT_CMD_INVALID;
  }

  return n;
}

GSM_STATIC void BtConnectTimeout(void *p)
{
  BtConnect_t *obj = (BtConnect_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void BtConnectObjectInit(BtConnect_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = BtConnectSerialize;
  this->atcmd.parse     = BtConnectParse;
  this->atcmd.timeout   = BtConnectTimeout;
  this->atcmd.timeoutInSec   = TIMEOUT_IN_SEC;
}

void BtConnectSetupRequest(BtConnect_t *this, uint32_t deviceId, uint32_t profileId)
{
  this->request.deviceId = deviceId;
  this->request.profileId = profileId;
}

AT_Command_t *BtConnectGetAtCommand(BtConnect_t *this)
{
  return &this->atcmd;
}

BtConnect_response_t BtConnectGetResponse(BtConnect_t *this)
{
  return this->response;
}

AT_CommandStatus_t BtConnectGetResponseStatus(BtConnect_t *this)
{
  return this->response.status;
}

bool BtConnectIsURC(const char *ibuf, size_t length)
{
    bool isConnecting = BtConnect_isBtConnecting(ibuf, length);
    bool isConnect = BtConnect_isBtConnect(ibuf, length);

    return isConnect || isConnecting;
}

size_t BtConnectParseURC(BtConnectURC_t *urc, const char *ibuf, size_t length)
{
    size_t offset = 0;

    if (BtConnect_isBtConnecting(ibuf, length)) {
        urc->type = BTCONNECT_CONNECTING;
        offset = BtConnect_parseBtConnecting(&urc->payload.connecting, ibuf, length);
    } else if (BtConnect_isBtConnect(ibuf, length)) {
        urc->type = BTCONNECT_CONNECT;
        offset = BtConnect_parseBtConnect(&urc->payload.connect, ibuf, length);
    } else {
        urc->type = BTCONNECT_NO_URC;
    }

    if (0 == offset)
        urc->type = BTCONNECT_NO_URC;

    return offset;
}

/****************************** END OF FILE **********************************/
