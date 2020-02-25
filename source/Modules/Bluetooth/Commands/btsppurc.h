/**
 * @file btsppurc.h
 * @brief
 */

#ifndef BTSPPURC_H
#define BTSPPURC_H

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
typedef struct Btsppurc_Request_s {
  uint32_t mode;
} Btsppurc_Request_t;

typedef struct Btsppurc_Response_s {
  AT_CommandStatus_t status;
} Btsppurc_Response_t;

typedef struct Btsppurc_s {
  Btsppurc_Request_t request;
  Btsppurc_Response_t response;
  AT_Command_t atcmd;
} Btsppurc_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void BtsppurcObjectInit(Btsppurc_t *this);

void BtsppurcSetupRequest(Btsppurc_t *this, uint32_t mode);

AT_Command_t *BtsppurcGetAtCommand(Btsppurc_t *this);

Btsppurc_Response_t BtsppurcGetResponse(Btsppurc_t *this);

AT_CommandStatus_t BtsppurcGetResponseStatus(Btsppurc_t *this);

#endif /* BTSPPURC_H */

/****************************** END OF FILE **********************************/
