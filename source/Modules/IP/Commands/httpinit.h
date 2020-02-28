/**
 * @file httpinit.h
 * @brief
 */

#ifndef HTTPINIT_H
#define HTTPINIT_H

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
typedef struct Httpinit_Request_s {
} Httpinit_Request_t;

typedef struct Httpinit_Response_s {
  AT_CommandStatus_t status;
} Httpinit_Response_t;

typedef struct Httpinit_s {
  Httpinit_Request_t request;
  Httpinit_Response_t response;
  AT_Command_t atcmd;
} Httpinit_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void HttpinitObjectInit(Httpinit_t *this);

void HttpinitSetupRequest(Httpinit_t *this);

AT_Command_t *HttpinitGetAtCommand(Httpinit_t *this);

Httpinit_Response_t HttpinitGetResponse(Httpinit_t *this);

AT_CommandStatus_t HttpinitGetResponseStatus(Httpinit_t *this);

#endif /* HTTPINIT_H */

/****************************** END OF FILE **********************************/
