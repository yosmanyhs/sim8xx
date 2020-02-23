/**
 * @file cipstart.h
 * @brief
 */

#ifndef CIPSTART_H
#define CIPSTART_H

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
  CIPSTART_NO_URC,
  CIPSTART_ALREADY_CONNECT,
  CIPSTART_CONNECT_OK,
  CIPSTART_CONNECT_FAIL,
} CipstartURC_Type_t; 

typedef struct CipstartURC_ConnectFail_s {
  uint32_t state;
} CipstartURC_ConnectFail_t;

typedef struct CipstartURC_s {
  CipstartURC_Type_t type;
  union {
    CipstartURC_ConnectFail_t fail;
  } payload;
} CipstartURC_t;

typedef struct Cipstart_Request_s {
  const char *protocol;
  const char *host;
  int32_t port;
} Cipstart_Request_t;

typedef struct Cipstart_Response_s {
  AT_CommandStatus_t status;
} Cipstart_Response_t;

typedef struct Cipstart_s {
  Cipstart_Request_t request;
  Cipstart_Response_t response;
  AT_Command_t atcmd;
} Cipstart_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void CipstartObjectInit(Cipstart_t *this);

void CipstartSetupRequest(Cipstart_t *this, const char *protocol, const char *host, int32_t port);

AT_Command_t *CipstartGetAtCommand(Cipstart_t *this);

Cipstart_Response_t CipstartGetResponse(Cipstart_t *this);

AT_CommandStatus_t CipstartGetResponseStatus(Cipstart_t *this);

bool CipstartIsURC(const char *ibuf, size_t ilen);

size_t CipstartParseURC(CipstartURC_t *urc, const char *ibuf, size_t ilen);

#endif /* CIPSTART_H */

/****************************** END OF FILE **********************************/
