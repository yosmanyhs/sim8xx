/**
 * @file Ate.h
 * @brief
 */

#ifndef ATE_H
#define ATE_H

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
typedef struct Ate_Request_s {
  uint8_t mode;
} Ate_Request_t;

typedef struct Ate_response_s {
  AT_CommandStatus_t status;
} Ate_Response_t;

typedef struct Ate_s {
  Ate_Request_t request;
  Ate_Response_t response;
  AT_Command_t atcmd;
} Ate_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void AteObjectInit(Ate_t *this);

void AteSetupRequest(Ate_t *this, uint8_t mode);

AT_Command_t *AteGetAtCommand(Ate_t *this);

Ate_Response_t AteGetResponse(Ate_t *this);

AT_CommandStatus_t AteGetResponseStatus(Ate_t *this);

#endif /* AT_H */

/****************************** END OF FILE **********************************/
