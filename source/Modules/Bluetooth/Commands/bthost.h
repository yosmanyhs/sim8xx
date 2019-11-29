/**
 * @file bthost.h
 * @brief 
 */

#ifndef BTHOST_H
#define BTHOST_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stdint.h>
#include <stddef.h>

#include "At.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef struct BtHost_request_s {
    const char *name;
} BtHost_request_t;

typedef struct BtHost_response_s {
    AT_CommandStatus_t status;
} BtHost_response_t;

typedef struct BtHost_s {
    BtHost_request_t request;
    BtHost_response_t response;
    AT_Command_t atcmd;
} BtHost_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void BtHostObjectInit(BtHost_t *this);

void BtHostSetupRequest(BtHost_t *this, const char *name);

AT_Command_t *BtHostGetAtCommand(BtHost_t *this);

BtHost_response_t BtHostGetResponse(BtHost_t *this);

AT_CommandStatus_t BtHostGetResponseStatus(BtHost_t *this);

#endif /* BTHOST_H */

/****************************** END OF FILE **********************************/