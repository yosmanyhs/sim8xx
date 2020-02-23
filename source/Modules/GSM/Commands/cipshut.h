/**
 * @file cipshut.h
 * @brief
 */

#ifndef CIPSHUT_H
#define CIPSHUT_H

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
typedef struct Cipshut_Request_s {
} Cipshut_Request_t;

typedef struct Cipshut_Response_s {
  AT_CommandStatus_t status;
} Cipshut_Response_t;

typedef struct Cipshut_s {
  Cipshut_Request_t request;
  Cipshut_Response_t response;
  AT_Command_t atcmd;
} Cipshut_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void CipshutObjectInit(Cipshut_t *this);

void CipshutSetupRequest(Cipshut_t *this);

AT_Command_t *CipshutGetAtCommand(Cipshut_t *this);

Cipshut_Response_t CipshutGetResponse(Cipshut_t *this);

AT_CommandStatus_t CipshutGetResponseStatus(Cipshut_t *this);

#endif /* CIPSHUT_H */

/****************************** END OF FILE **********************************/
