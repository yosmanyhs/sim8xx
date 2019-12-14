/**
 * @file btpaircfg.h
 * @brief
 */

#ifndef BTPAIRCFG_H
#define BTPAIRCFG_H

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
typedef struct BtPaircfg_request_s {
  uint8_t mode;
  const char *pin;
} BtPaircfg_request_t;

typedef struct BtPaircfg_response_s {
  AT_CommandStatus_t status;
} BtPaircfg_response_t;

typedef struct BtPaircfg_s {
  BtPaircfg_request_t request;
  BtPaircfg_response_t response;
  AT_Command_t atcmd;
} BtPaircfg_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void BtPaircfgObjectInit(BtPaircfg_t *this);

void BtPaircfgSetupRequest(BtPaircfg_t *this, uint8_t mode, const char *pin);

AT_Command_t *BtPaircfgGetAtCommand(BtPaircfg_t *this);

BtPaircfg_response_t BtPaircfgGetResponse(BtPaircfg_t *this);

AT_CommandStatus_t BtPaircfgGetResponseStatus(BtPaircfg_t *this);

#endif /* BTPAIRCFG_H */

/****************************** END OF FILE **********************************/