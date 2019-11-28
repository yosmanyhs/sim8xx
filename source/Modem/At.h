/**
 * @file At.h
 * @brief 
 */

#ifndef AT_H
#define AT_H

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
    AT_CMD_INVALID,
    AT_CMD_OK,
    AT_CMD_CONNECT,
    AT_CMD_RING,
    AT_CMD_NO_CARRIER,
    AT_CMD_ERROR,
    AT_CMD_NO_DIALTONE,
    AT_CMD_BUSY,
    AT_CMD_NO_ANSWER,
    AT_CMD_PROCEEDING,
    AT_CMD_WAIT_FOR_USER_DATA,
} AT_CommandStatus_t;

typedef size_t (*AT_Serialize_t)(void *obj, char *obuf, size_t length);

typedef size_t (*AT_Parse_t)(void *obj, const char *ibuf, size_t length);

typedef struct AT_Command_s {
    void *obj;
    AT_Serialize_t serialize;
    AT_Parse_t parse;
    uint32_t timeout;
} AT_Command_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
size_t AT_CommandStatusParse(const char *ibuf, size_t length, AT_CommandStatus_t *status);

#endif /* AT_H */

/****************************** END OF FILE **********************************/
