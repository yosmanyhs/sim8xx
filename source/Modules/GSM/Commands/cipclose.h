/**
 * @file cipclose.h
 * @brief
 */

#ifndef CIPCLOSE_H
#define CIPCLOSE_H

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
typedef struct Cipclose_Request_s {
} Cipclose_Request_t;

typedef struct Cipclose_Response_s {
  AT_CommandStatus_t status;
} Cipclose_Response_t;

typedef struct Cipclose_s {
  Cipclose_Request_t request;
  Cipclose_Response_t response;
  AT_Command_t atcmd;
} Cipclose_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void CipcloseObjectInit(Cipclose_t *this);

void CipcloseSetupRequest(Cipclose_t *this);

AT_Command_t *CipcloseGetAtCommand(Cipclose_t *this);

Cipclose_Response_t CipcloseGetResponse(Cipclose_t *this);

AT_CommandStatus_t CipcloseGetResponseStatus(Cipclose_t *this);

#endif /* CIPCLOSE_H */

/****************************** END OF FILE **********************************/
