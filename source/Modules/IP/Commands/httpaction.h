/**
 * @file httpaction.h
 * @brief
 */

#ifndef HTTPACTION_H
#define HTTPACTION_H

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
  HTTPACTION_GET   = 0,
  HTTPACTION_POST  = 1,
  HTTPACTION_HEAD  = 2,
} HttpactionMethod_t;

typedef struct Httpaction_Request_s {
  HttpactionMethod_t method;
} Httpaction_Request_t;

typedef struct Httpaction_Response_s {
  AT_CommandStatus_t status;
} Httpaction_Response_t;

typedef struct Httpaction_s {
  Httpaction_Request_t request;
  Httpaction_Response_t response;
  AT_Command_t atcmd;
} Httpaction_t;

typedef struct HttpactionURC_s {
  HttpactionMethod_t method;
  int32_t httpStatus;
  int32_t dataLength;
} HttpactionURC_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void HttpactionObjectInit(Httpaction_t *this);

void HttpactionSetupRequest(Httpaction_t *this, HttpactionMethod_t method);

AT_Command_t *HttpactionGetAtCommand(Httpaction_t *this);

Httpaction_Response_t HttpactionGetResponse(Httpaction_t *this);

AT_CommandStatus_t HttpactionGetResponseStatus(Httpaction_t *this);

bool HttpactionIsURC(const char *ibuf, size_t ilen);

size_t HttpactionParseURC(HttpactionURC_t *urc, const char *ibuf, size_t ilen);

#endif /* HTTPACTION_H */

/****************************** END OF FILE **********************************/
