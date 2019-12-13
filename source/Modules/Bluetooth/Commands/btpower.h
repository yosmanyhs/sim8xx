/**
 * @file btpower.h
 * @brief
 */

#ifndef BTPOWER_H
#define BTPOWER_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "AtCommand.h"

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
typedef struct BtPower_request_s {
  uint8_t mode;
} BtPower_request_t;

typedef struct BtPower_response_s {
  AT_CommandStatus_t status;
} BtPower_response_t;

typedef struct BtPower_s {
  BtPower_request_t request;
  BtPower_response_t response;
  AT_Command_t atcmd;
} BtPower_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void BtPowerObjectInit(BtPower_t *this);

void BtPowerSetupRequest(BtPower_t *this, uint8_t mode);

AT_Command_t *BtPowerGetAtCommand(BtPower_t *this);

BtPower_response_t BtPowerGetResponse(BtPower_t *this);

AT_CommandStatus_t BtPowerGetResponseStatus(BtPower_t *this);

#endif /* BTPOWER_H */

/****************************** END OF FILE **********************************/