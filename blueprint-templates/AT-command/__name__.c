/**
 * @file {{name}}.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "{{name}}.h"
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

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
GSM_STATIC size_t {{pascalCase name}}Serialize(void *p, char *obuf, size_t olen)
{
  {{pascalCase name}}_t *obj = ({{pascalCase name}}_t*)p;
  
  memset(obuf, 0, olen);

  // TODO add implementation

  return strlen(obuf);
}

GSM_STATIC size_t {{pascalCase name}}Parse(void *p, const char *ibuf, size_t ilen)
{
  {{pascalCase name}}_t *obj = ({{pascalCase name}}_t*)p;
  AT_CommandStatus_t status = AT_CMD_INVALID;

  size_t n = AT_CommandStatusParse(ibuf, ilen, &status);

  size_t offset = 0;
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

  return offset;
}

GSM_STATIC void {{pascalCase name}}Timeout(void *p)
{
  {{pascalCase name}}_t *obj = ({{pascalCase name}}_t*)p;
  obj->response.status = AT_CMD_TIMEOUT;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void {{pascalCase name}}ObjectInit({{pascalCase name}}_t *this)
{
  memset(this, 0, sizeof(*this));
  this->atcmd.obj               = this;
  this->atcmd.serialize         = {{pascalCase name}}Serialize;
  this->atcmd.parse             = {{pascalCase name}}Parse;
  this->atcmd.timeout           = {{pascalCase name}}Timeout;
  this->atcmd.timeoutInMilliSec = TIMEOUT_IN_MSEC;
}

void {{pascalCase name}}SetupRequest({{pascalCase name}}_t *this)
{
  (void)this;
}

AT_Command_t *{{pascalCase name}}GetAtCommand({{pascalCase name}}_t *this)
{
  return &this->atcmd;
}

{{pascalCase name}}_Response_t {{pascalCase name}}GetResponse({{pascalCase name}}_t *this)
{
  return this->response;
}

AT_CommandStatus_t {{pascalCase name}}GetResponseStatus({{pascalCase name}}_t *this)
{
  return this->response.status;
}

/****************************** END OF FILE **********************************/
