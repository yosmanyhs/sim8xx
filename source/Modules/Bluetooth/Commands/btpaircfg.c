/**
 * @file btpower.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "btpaircfg.h"

#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define TIMEOUT_IN_SEC 2

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
    BtPaircfgSerialize(void *p, char *obuf, size_t length)
{
  BtPaircfg_t *obj = (BtPaircfg_t *)p;
  uint8_t mode     = obj->request.mode;
  const char *pin  = obj->request.pin;

  size_t n = 0;
  if (20 < length) {
    char c = '0';
    if ((0 == mode) || (1 == mode) || (2 == mode))
      c += mode;

    strncat(obuf, "AT+BTPAIRCFG=", 13);
    obuf[13] = c;
    obuf[14] = '\0';
    if (pin && (1 == mode)) {
      strncat(obuf, ",", 1);
      strncat(obuf, pin, 4);
    }
    n = strlen(obuf);
  }

  return n;
}

#if !defined(TEST)
static
#endif
    size_t
    BtPaircfgParse(void *p, const char *ibuf, size_t length)
{
  BtPaircfg_t *obj          = (BtPaircfg_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;
  size_t n                  = AT_CommandStatusParse(ibuf, length, &status);

  if (n && ((AT_CMD_OK == status) || (AT_CMD_ERROR == status))) {
    obj->response.status = status;
  } else {
    n = 0;
  }

  return n;
}

#if !defined(TEST)
static
#endif
void BtPaircfgTimeout(void *p)
{
  BtPaircfg_t *obj = (BtPaircfg_t *)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void BtPaircfgObjectInit(BtPaircfg_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = BtPaircfgSerialize;
  this->atcmd.parse     = BtPaircfgParse;
  this->atcmd.timeout   = BtPaircfgTimeout;
  this->atcmd.timeoutInSec   = TIMEOUT_IN_SEC;
}

void BtPaircfgSetupRequest(BtPaircfg_t *this, uint8_t mode, const char *pin)
{
  this->request.mode = mode;
  this->request.pin  = pin;
}

AT_Command_t *BtPaircfgGetAtCommand(BtPaircfg_t *this)
{
  return &this->atcmd;
}

BtPaircfg_response_t BtPaircfgGetResponse(BtPaircfg_t *this)
{
  return this->response;
}

AT_CommandStatus_t BtPaircfgGetResponseStatus(BtPaircfg_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
