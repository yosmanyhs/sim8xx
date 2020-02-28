/**
 * @file httpterm.h
 * @brief
 */

#ifndef HTTPTERM_H
#define HTTPTERM_H

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
typedef struct Httpterm_Request_s {
} Httpterm_Request_t;

typedef struct Httpterm_Response_s {
  AT_CommandStatus_t status;
} Httpterm_Response_t;

typedef struct Httpterm_s {
  Httpterm_Request_t request;
  Httpterm_Response_t response;
  AT_Command_t atcmd;
} Httpterm_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void HttptermObjectInit(Httpterm_t *this);

void HttptermSetupRequest(Httpterm_t *this);

AT_Command_t *HttptermGetAtCommand(Httpterm_t *this);

Httpterm_Response_t HttptermGetResponse(Httpterm_t *this);

AT_CommandStatus_t HttptermGetResponseStatus(Httpterm_t *this);

#endif /* HTTPTERM_H */

/****************************** END OF FILE **********************************/
