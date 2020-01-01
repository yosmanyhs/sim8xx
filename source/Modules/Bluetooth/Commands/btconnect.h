/**
 * @file btconnect.h
 * @brief
 */

#ifndef BTCONNECT_H
#define BTCONNECT_H

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
typedef enum {
  BTCONNECT_NO_URC,
  BTCONNECT_CONNECTING,
  BTCONNECT_CONNECT,
} BtConnectURC_Type_t;

typedef struct BtConnect_Result_s {
  int32_t id;
  char name[19];
  char address[19];
  char profile[10];
} BtConnect_Result_t;

typedef struct BtConnect_ConnectingURC_s {
  char address[19];
  char profile[10];
} BtConnect_ConnectingURC_t;

typedef struct BtConnectURC_s {
    BtConnectURC_Type_t type;
    union {
        BtConnect_ConnectingURC_t connecting;
        BtConnect_Result_t connect;
    } payload;    
} BtConnectURC_t;

typedef struct BtConnect_request_s {
  int32_t deviceId;
  int32_t profileId;
} BtConnect_request_t;

typedef struct BtConnect_response_s {
  AT_CommandStatus_t status;
  BtConnect_Result_t result;
} BtConnect_response_t;

typedef struct BtConnect_s {
  BtConnect_request_t request;
  BtConnect_response_t response;
  AT_Command_t atcmd;
} BtConnect_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void BtConnectObjectInit(BtConnect_t *this);

void BtConnectSetupRequest(BtConnect_t *this, uint32_t deviceId, uint32_t profileId);

AT_Command_t *BtConnectGetAtCommand(BtConnect_t *this);

BtConnect_response_t BtConnectGetResponse(BtConnect_t *this);

AT_CommandStatus_t BtConnectGetResponseStatus(BtConnect_t *this);

bool BtConnectIsURC(const char *ibuf, size_t length);

size_t BtConnectParseURC(BtConnectURC_t *urc, const char *ibuf, size_t length);

#endif /* BTCONNECT_H */

/****************************** END OF FILE **********************************/
