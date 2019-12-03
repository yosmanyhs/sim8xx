/**
 * @file btconnect.h
 * @brief
 */

#ifndef BTCONNECT_H
#define BTCONNECT_H

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
typedef struct BtConnectingURC_s {
    char address[19];
    char profile[10];
} BtConnectingURC_t;

typedef struct BtConnectURC_s {
    uint32_t id;
    char name[19];
    char address[19];
    char profile[10];
} BtConnectURC_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
bool BtConnectIsURC_BtConnecting(const char *ibuf, size_t length);

size_t BtConnectParseURC_BtConnecting(BtConnectingURC_t *urc, const char *ibuf, size_t length);

bool BtConnectIsURC_BtConnect(const char *ibuf, size_t length);

size_t BtConnectParseURC_BtConnect(BtConnectURC_t *urc, const char *ibuf, size_t length);

#endif /* BTCONNECT_H */

/****************************** END OF FILE **********************************/
