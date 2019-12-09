/**
 * @file btpower.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "btpower.h"

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
    BtPowerSerialize(void *p, char *obuf, size_t length)
{
  BtPower_t *obj = (BtPower_t *)p;
  size_t n       = 0;
  if (13 < length) {
    strncat(obuf, "AT+BTPOWER=", 11);
    strncat(obuf, obj->request.mode == 1 ? "1" : "0", 1);
    n = strlen(obuf);
  }

  return n;
}

#if !defined(TEST)
static
#endif
    size_t
    BtPowerParse(void *p, const char *ibuf, size_t length)
{
  BtPower_t *obj            = (BtPower_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;
  size_t n                  = AT_CommandStatusParse(ibuf, length, &status);

  if (n && ((AT_CMD_OK == status) || (AT_CMD_ERROR == status))) {
    obj->response.status = status;
  } else {
    n = 0;
  }

  return n;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void BtPowerObjectInit(BtPower_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = BtPowerSerialize;
  this->atcmd.parse     = BtPowerParse;
  this->atcmd.timeout   = TIMEOUT_IN_SEC;
}

void BtPowerSetupRequest(BtPower_t *this, uint8_t mode)
{
  this->request.mode = mode;
}

AT_Command_t *BtPowerGetAtCommand(BtPower_t *this)
{
  return &this->atcmd;
}

BtPower_response_t BtPowerGetResponse(BtPower_t *this)
{
  return this->response;
}

AT_CommandStatus_t BtPowerGetResponseStatus(BtPower_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
