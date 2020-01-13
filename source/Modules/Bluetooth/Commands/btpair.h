/**
 * @file Btpair.h
 * @brief
 */

#ifndef BTPAIR_H
#define BTPAIR_H

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
  BTPAIR_NO_URC,
  BTPAIR_PASSIVE_MODE_WITH_SUCCESS,
} BtPairURC_Type_t;

typedef struct BtPairPassiveSuccess_s {
  int32_t id;
  char name[20];
  char address[18];
} BtPairPassiveSuccess_t;

typedef struct BtPairURC_s {
  BtPairURC_Type_t type;
  union {
    BtPairPassiveSuccess_t passiveSuccess;
  } payload;    
} BtPairURC_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
bool BtPairIsURC(const char *ibuf, size_t ilen);

size_t BtPairParseURC(BtPairURC_t *urc, const char *ibuf, size_t ilen);

#endif /* BTPAIR_H */

/****************************** END OF FILE **********************************/
