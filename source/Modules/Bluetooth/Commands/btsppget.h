/**
 * @file btsppget.h
 * @brief
 */

#ifndef BT_SPP_GET_H
#define BT_SPP_GET_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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
  BTSPPGET_NO_URC,
  BTSPPGET_SPPDATA,
} BtSppGetURC_Type_t;

typedef struct BtSppGetDataURC_s {
  uint32_t id;
  size_t length;
  char data[128];
} BtSppGetDataURC_t;

typedef struct BtSppGetURC_s {
    BtSppGetURC_Type_t type;
    union {
        BtSppGetDataURC_t getdata;
    } payload;
} BtSppGetURC_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
bool BtSppGetIsURC(const char *ibuf, size_t length);

size_t BtSppGetParseURC(BtSppGetURC_t *urc, const char *ibuf, size_t length);

#endif /* BT_SPP_GET_H */

/****************************** END OF FILE **********************************/
