/**
 * @file btsppsend.h
 * @brief
 */

#ifndef BTSPPSEND_H
#define BTSPPSEND_H

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
typedef struct BtSppSend_request_s {
  const char *data;
  size_t length;
} BtSppSend_request_t;

typedef struct BtSppSend_response_s {
  AT_CommandStatus_t status;
} BtSppSend_response_t;

typedef enum {
  BT_SPP_SEND_STATE_COMMAND,
  BT_SPP_SEND_STATE_DATA,
} BtSppSendState_t;

typedef struct BtSppSend_s {
  BtSppSend_request_t request;
  BtSppSend_response_t response;
  BtSppSendState_t state;
  AT_Command_t atcmd;
} BtSppSend_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void BtSppSendObjectInit(BtSppSend_t *this);

void BtSppSendSetupRequest(BtSppSend_t *this, const char *data, size_t length);

void BtSppSendSetCommandMode(BtSppSend_t *this);

void BtSppSendSetDataMode(BtSppSend_t *this);

AT_Command_t *BtSppSendGetAtCommand(BtSppSend_t *this);

BtSppSend_response_t BtSppSendGetResponse(BtSppSend_t *this);

AT_CommandStatus_t BtSppSendGetResponseStatus(BtSppSend_t *this);

#endif /* BTSPPSEND_H */

/****************************** END OF FILE **********************************/