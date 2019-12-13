/**
 * @file At.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "At.h"

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
#if !defined(TEST)
static
#endif
    size_t
    AtSerialize(void *p, char *obuf, size_t length)
{
  memset(obuf, 0, length);

  At_t *obj = (At_t *)p;
  size_t n      = 0;
  if (2 < length) {
    strncpy(obuf, "AT", length);
    n = strlen(obuf);
  }

  return n;
}

#if !defined(TEST)
static
#endif
    size_t
    AtParse(void *p, const char *ibuf, size_t length)
{
  At_t *obj             = (At_t *)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;
  size_t n                  = AT_CommandStatusParse(ibuf, length, &status);

  if (n && (AT_CMD_OK == status)) {
    obj->response.status = status;
  } else {
    n = 0;
  }

  return n;
}
/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void AtObjectInit(At_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj       = this;
  this->atcmd.serialize = AtSerialize;
  this->atcmd.parse     = AtParse;
  this->atcmd.timeout   = TIMEOUT_IN_SEC;
}

void AtSetupRequest(At_t *this)
{
}

AT_Command_t *AtGetAtCommand(At_t *this)
{
  return &this->atcmd;
}

At_response_t AtGetResponse(At_t *this)
{
  return this->response;
}

AT_CommandStatus_t AtGetResponseStatus(At_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
