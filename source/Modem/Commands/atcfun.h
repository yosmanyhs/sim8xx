/**
 * @file atcfun.h
 * @brief
 */

#ifndef ATCFUN_H
#define ATCFUN_H

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
  ATCFUN_NO_URC,
  ATCFUN_INFO,
} AtCfunURC_Type_t;

typedef enum {
  ATCFUN_LEVEL_INVALID,
  ATCFUN_LEVEL_MINIMUM,
  ATCFUN_LEVEL_FULL,
  ATCFUN_LEVEL_DISABLE_PHONE,
} AtCfunLevel_t;

typedef struct AtCfunInfo_s {
  AtCfunLevel_t code;
} AtCfunInfo_t;

typedef struct AtCfunURC_s {
  AtCfunURC_Type_t type;
  union {
    AtCfunInfo_t info;
  } payload;
} AtCfunURC_t;

typedef struct AtCfun_Request_s {
  int32_t fun;
  int32_t rst;
} AtCfun_Request_t;

typedef struct AtCfun_Response_s {
  AT_CommandStatus_t status;
} AtCfun_Response_t;

typedef struct AtCfun_s {
  AtCfun_Request_t request;
  AtCfun_Response_t response;
  AT_Command_t atcmd;
} AtCfun_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void AtCfunObjectInit(AtCfun_t *this);

void AtCfunSetupRequest(AtCfun_t *this, int32_t fun, int32_t rst);

AT_Command_t *AtCfunGetAtCommand(AtCfun_t *this);

AtCfun_Response_t AtCfunGetResponse(AtCfun_t *this);

AT_CommandStatus_t AtCfunGetResponseStatus(AtCfun_t *this);

bool AtCfunIsURC(const char *ibuf, size_t ilen);

size_t AtCfunParseURC(AtCfunURC_t *urc, const char *ibuf, size_t ilen);

#endif /* ATCFUN_H */

/****************************** END OF FILE **********************************/
