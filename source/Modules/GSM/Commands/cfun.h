/**
 * @file cfun.h
 * @brief
 */

#ifndef CFUN_H
#define CFUN_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Common/AtCommand.h"

#include <stdbool.h>
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
  CFUN_NO_URC,
  CFUN_INFO,
} CfunURC_Type_t;

typedef enum {
  CFUN_LEVEL_INVALID,
  CFUN_LEVEL_MINIMUM,
  CFUN_LEVEL_FULL,
  CFUN_LEVEL_DISABLE_PHONE,
} CfunLevel_t;

typedef struct CfunInfo_s {
  CfunLevel_t code;
} CfunInfo_t;

typedef struct CfunURC_s {
  CfunURC_Type_t type;
  union {
    CfunInfo_t info;
  } payload;
} CfunURC_t;

typedef struct Cfun_Request_s {
  int32_t fun;
  int32_t rst;
} Cfun_Request_t;

typedef struct Cfun_Response_s {
  AT_CommandStatus_t status;
} Cfun_Response_t;

typedef struct Cfun_s {
  Cfun_Request_t request;
  Cfun_Response_t response;
  AT_Command_t atcmd;
} Cfun_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void CfunObjectInit(Cfun_t *this);

void CfunSetupRequest(Cfun_t *this, int32_t fun, int32_t rst);

AT_Command_t *CfunGetAtCommand(Cfun_t *this);

Cfun_Response_t CfunGetResponse(Cfun_t *this);

AT_CommandStatus_t CfunGetResponseStatus(Cfun_t *this);

bool CfunIsURC(const char *ibuf, size_t ilen);

size_t CfunParseURC(CfunURC_t *urc, const char *ibuf, size_t ilen);

#endif /* CFUN_H */

/****************************** END OF FILE **********************************/
