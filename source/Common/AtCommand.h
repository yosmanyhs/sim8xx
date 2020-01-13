/**
 * @file AtCommand.h
 * @brief
 */

#ifndef ATCOMMAND_H
#define ATCOMMAND_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define CRLF "\r\n"
#define CRLFLENGTH 2

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef enum {
  AT_CMD_INVALID,
  AT_CMD_TIMEOUT,
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
  AT_CMD_SEND_OK,
  AT_CMD_SEND_FAIL,
} AT_CommandStatus_t;

typedef size_t (*AT_Serialize_t)(void *p, char *obuf, size_t olen);

typedef size_t (*AT_Parse_t)(void *p, const char *ibuf, size_t ilen);

typedef void (*AT_Timeout)(void *p);

typedef struct AT_Command_s {
  void *obj;
  AT_Serialize_t serialize;
  AT_Parse_t parse;
  AT_Timeout timeout;
  uint32_t timeoutInMilliSec;
} AT_Command_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
size_t AT_CommandStatusParse(const char *ibuf, size_t ilen, AT_CommandStatus_t *status);

#endif /* ATCOMMAND_H */

/****************************** END OF FILE **********************************/
