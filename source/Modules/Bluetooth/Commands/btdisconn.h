/**
 * @file btdisconn.h
 * @brief
 */

#ifndef BTDISCONN_H
#define BTDISCONN_H

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
typedef enum {
  BTDISCONN_NO_URC,
  BTDISCONN_DISCONNECTED,
} BtDisconnURC_Type_t;

typedef struct BtDisconn_DisconnectedURC_s {
  char name[19];
  char address[19];
  char profile[10];
} BtDisconn_DisconnectedURC_t;

typedef struct BtDisconnURC_s {
    BtDisconnURC_Type_t type;
    union {
        BtDisconn_DisconnectedURC_t disconn;
    } payload;
} BtDisconnURC_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
bool BtDisconnIsURC(const char *ibuf, size_t length);

size_t BtDisconnParseURC(BtDisconnURC_t *urc, const char *ibuf, size_t length);

#endif /* BTDISCONN_H */

/****************************** END OF FILE **********************************/
