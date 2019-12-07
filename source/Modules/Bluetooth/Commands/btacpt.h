/**
 * @file btacpt.h
 * @brief
 */

#ifndef BTACPT_H
#define BTACPT_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "At.h"

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
typedef struct BtAcpt_request_s {
  uint8_t mode;
} BtAcpt_request_t;

typedef struct BtAcpt_response_s {
  AT_CommandStatus_t status;
} BtAcpt_response_t;

typedef struct BtAcpt_s {
  BtAcpt_request_t request;
  BtAcpt_response_t response;
  AT_Command_t atcmd;
} BtAcpt_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void BtAcptObjectInit(BtAcpt_t *this);

void BtAcptSetupRequest(BtAcpt_t *this, uint8_t mode);

AT_Command_t *BtAcptGetAtCommand(BtAcpt_t *this);

BtAcpt_response_t BtAcptGetResponse(BtAcpt_t *this);

AT_CommandStatus_t BtAcptGetResponseStatus(BtAcpt_t *this);

#endif /* BTACPT_H */

/****************************** END OF FILE **********************************/
