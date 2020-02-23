/**
 * @file cifsr.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "cifsr.h"
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

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
GSM_STATIC size_t CifsrSerialize(void *p, char *obuf, size_t olen)
{
  (void)p;
  
  memset(obuf, 0, olen);
  strncpy(obuf, "AT+CIFSR\r", olen);

  return strlen(obuf);
}

GSM_STATIC size_t CifsrParse(void *p, const char *ibuf, size_t ilen)
{
  Cifsr_t *obj = (Cifsr_t*)p;

  memset(&obj->response, 9, sizeof(obj->response));

  AT_CommandStatus_t status = AT_CMD_INVALID;
  size_t offset = AT_CommandStatusParse(ibuf, ilen, &status);

  if ((0 < offset) && (AT_CMD_ERROR == status)) {
    obj->response.status = AT_CMD_ERROR;
    return offset;
  } else {
    offset = 0;
    char *data = obj->response.ipaddr;
    size_t dlen = sizeof(obj->response.ipaddr);
    const char *next = ibuf;
    const char *end  = ibuf + ilen;

    size_t n = GSM_UtilsGetString(next, end - next, data, dlen, '\n', '.');
    if (n) {
      offset += n;
      next += n;
      strncat(data, ".", dlen);
      data += strlen(data);
      dlen -= strlen(data);
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }

    n = GSM_UtilsGetString(next, end - next, data, dlen, '.', '.');
    if (n) {
      offset += n;
      next += n;
      strncat(data, ".", dlen);
      data += strlen(data);
      dlen -= strlen(data);
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }

    n = GSM_UtilsGetString(next, end - next, data, dlen, '.', '.');
    if (n) {
      offset += n;
      next += n;
      strncat(data, ".", dlen);
      data += strlen(data);
      dlen -= strlen(data);
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }

    n = GSM_UtilsGetString(next, end - next, data, dlen, '.', '\r');
    if (n) {
      offset += n;
      next += n;
      data += strlen(data);
      dlen -= strlen(data);
    } else {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    }

    if (strncasecmp(CRLF, next, CRLFLENGTH)) {
      memset(&obj->response, 0, sizeof(obj->response));
      return 0;
    } else {
      offset += CRLFLENGTH;
      obj->response.status = AT_CMD_OK;
    }
  }

  return offset;
}

GSM_STATIC void CifsrTimeout(void *p)
{
  Cifsr_t *obj = (Cifsr_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void CifsrObjectInit(Cifsr_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = CifsrSerialize;
  this->atcmd.parse             = CifsrParse;
  this->atcmd.timeout           = CifsrTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void CifsrSetupRequest(Cifsr_t *this)
{
  (void)this;
}

AT_Command_t *CifsrGetAtCommand(Cifsr_t *this)
{
  return &this->atcmd;
}

Cifsr_Response_t CifsrGetResponse(Cifsr_t *this)
{
  return this->response;
}

AT_CommandStatus_t CifsrGetResponseStatus(Cifsr_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
