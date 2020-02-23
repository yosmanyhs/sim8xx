/**
 * @file ciicr.h
 * @brief
 */

#ifndef CIICR_H
#define CIICR_H

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
typedef struct Ciicr_Response_s {
  AT_CommandStatus_t status;
} Ciicr_Response_t;

typedef struct Ciicr_s {
  Ciicr_Response_t response;
  AT_Command_t atcmd;
} Ciicr_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void CiicrObjectInit(Ciicr_t *this);

void CiicrSetupRequest(Ciicr_t *this);

AT_Command_t *CiicrGetAtCommand(Ciicr_t *this);

Ciicr_Response_t CiicrGetResponse(Ciicr_t *this);

AT_CommandStatus_t CiicrGetResponseStatus(Ciicr_t *this);

#endif /* CIICR_H */

/****************************** END OF FILE **********************************/
