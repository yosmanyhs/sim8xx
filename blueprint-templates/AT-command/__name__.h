/**
 * @file {{name}}.h
 * @brief
 */

#ifndef {{upperSnakeCase name}}_H
#define {{upperSnakeCase name}}_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Common/AtCommand.h"

#include <stddef.h>
#include <stdint.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef struct {{pascalCase name}}_Request_s {
} {{pascalCase name}}_Request_t;

typedef struct {{pascalCase name}}_Response_s {
  AT_CommandStatus_t status;
} {{pascalCase name}}_Response_t;

typedef struct {{pascalCase name}}_s {
  {{pascalCase name}}_Request_t request;
  {{pascalCase name}}_Response_t response;
  AT_Command_t atcmd;
} {{pascalCase name}}_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void {{pascalCase name}}ObjectInit({{pascalCase name}}_t *this);

void {{pascalCase name}}SetupRequest({{pascalCase name}}_t *this);

AT_Command_t *{{pascalCase name}}GetAtCommand({{pascalCase name}}_t *this);

{{pascalCase name}}_Response_t {{pascalCase name}}GetResponse({{pascalCase name}}_t *this);

AT_CommandStatus_t {{pascalCase name}}GetResponseStatus({{pascalCase name}}_t *this);

#endif /* {{upperSnakeCase name}}_H */

/****************************** END OF FILE **********************************/
