/**
 * @file cifsr.h
 * @brief
 */

#ifndef CIFSR_H
#define CIFSR_H

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
typedef struct Cifsr_Request_s {
} Cifsr_Request_t;

typedef struct Cifsr_Response_s {
  char ipaddr[16];
  AT_CommandStatus_t status;
} Cifsr_Response_t;

typedef struct Cifsr_s {
  Cifsr_Request_t request;
  Cifsr_Response_t response;
  AT_Command_t atcmd;
} Cifsr_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void CifsrObjectInit(Cifsr_t *this);

void CifsrSetupRequest(Cifsr_t *this);

AT_Command_t *CifsrGetAtCommand(Cifsr_t *this);

Cifsr_Response_t CifsrGetResponse(Cifsr_t *this);

AT_CommandStatus_t CifsrGetResponseStatus(Cifsr_t *this);

#endif /* CIFSR_H */

/****************************** END OF FILE **********************************/
