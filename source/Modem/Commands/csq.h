/**
 * @file csq.h
 * @brief
 */

#ifndef CSQ_H
#define CSQ_H

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
typedef struct Csq_Request_s {
} Csq_Request_t;

typedef struct Csq_Response_s {
  AT_CommandStatus_t status;
  int32_t rssi;
  int32_t ber;
} Csq_Response_t;

typedef struct Csq_s {
  Csq_Request_t request;
  Csq_Response_t response;
  AT_Command_t atcmd;
} Csq_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void CsqObjectInit(Csq_t *this);

void CsqSetupRequest(Csq_t *this);

AT_Command_t *CsqGetAtCommand(Csq_t *this);

Csq_Response_t CsqGetResponse(Csq_t *this);

AT_CommandStatus_t CsqGetResponseStatus(Csq_t *this);

#endif /* CSQ_H */

/****************************** END OF FILE **********************************/
