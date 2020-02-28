/**
 * @file httppara.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "httppara.h"
#include "Common/Env.h"
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
static const char *key_cid = "CID";
static const char *key_url = "URL";

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
GSM_STATIC size_t HttpparaSerialize(void *p, char *obuf, size_t olen)
{
  Httppara_t *obj = (Httppara_t*)p;
  
  memset(obuf, 0, olen);

  strncpy(obuf, "AT+HTTPPARA=\"", olen - 1);
  strncat(obuf, obj->request.key, olen - 1 - strlen(obuf));
  strncat(obuf, "\",", olen - 1 - strlen(obuf));
  if (key_cid != obj->request.key) {
    strncat(obuf, "\"", olen - 1 - strlen(obuf));
  }
  strncat(obuf, obj->request.value, olen - 1 - strlen(obuf));
  if (key_cid != obj->request.key) {
    strncat(obuf, "\"", olen - 1 - strlen(obuf));
  }
  strncat(obuf, "\r", olen - 1 - strlen(obuf));

  return strlen(obuf);
}

GSM_STATIC size_t HttpparaParse(void *p, const char *ibuf, size_t ilen)
{
  Httppara_t *obj = (Httppara_t*)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t offset = 0;
  size_t n = AT_CommandStatusParse(ibuf, ilen, &status);

  if (AT_CMD_OK == status) {
    obj->response.status = status;
    offset = n;
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

GSM_STATIC void HttpparaTimeout(void *p)
{
  Httppara_t *obj = (Httppara_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void HttpparaObjectInit(Httppara_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = HttpparaSerialize;
  this->atcmd.parse             = HttpparaParse;
  this->atcmd.timeout           = HttpparaTimeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void HttpparaSetupRequestSetCid(Httppara_t *this, const char *cid)
{
  this->request.key = key_cid;
  this->request.value = cid;
}

void HttpparaSetupRequestSetUrl(Httppara_t *this, const char *url)
{
  this->request.key = key_url;
  this->request.value = url;
}

AT_Command_t *HttpparaGetAtCommand(Httppara_t *this)
{
  return &this->atcmd;
}

Httppara_Response_t HttpparaGetResponse(Httppara_t *this)
{
  return this->response;
}

AT_CommandStatus_t HttpparaGetResponseStatus(Httppara_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
