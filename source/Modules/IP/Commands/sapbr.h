/**
 * @file sapbr.h
 * @brief
 */

#ifndef SAPBR_H
#define SAPBR_H

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
  CMD_TYPE_CLOSE,
  CMD_TYPE_OPEN,
  CMD_TYPE_QUERY,
  CMD_TYPE_SET,
  CMD_TYPE_GET,
} CmdType_t;

typedef enum {
  BEARER_STATUS_CONNECTING,
  BEARER_STATUS_CONNECTED,
  BEARER_STATUS_CLOSING,
  BEARER_STATUS_CLOSED,
} BearerStatus_t;

typedef enum {
  PARAM_TAG_CONNTYPE,
  PARAM_TAG_APN,
  PARAM_TAG_USER,
  PARAM_TAG_PWD,
  PARAM_TAG_PHONENUM,
  PARAM_TAG_RATE,
} ParamTag_t;

typedef enum {
  CONNTYPE_CSD,
  CONNTYPE_GPRS,
} ConnectionType_t;

typedef enum {
  RATE_VALUE_2400,
  RATE_VALUE_4800,
  RATE_VALUE_9600,
  RATE_VALUE_14400,
} RateValue_t;

typedef struct Sapbr_Request_s {
  CmdType_t cmd;
  int32_t id;
  ParamTag_t tag;
  const char *value;
} Sapbr_Request_t;

typedef struct Sapbr_Response_s {
  AT_CommandStatus_t status;
  union {
    struct Query_s {
      int32_t id;
      int32_t status;
      char ipaddr[4 * 3 + 3 + 1];
    } query;
    struct Get_st {
      char tag[20];
      char value[50];
    } get;
  } payload;
} Sapbr_Response_t;

typedef struct Sapbr_s {
  Sapbr_Request_t request;
  Sapbr_Response_t response;
  AT_Command_t atcmd;
} Sapbr_t;

typedef enum {
  SAPBR_URC_NO_URC,
  SAPBR_URC_DEACT,
} SapbrURC_Type_t;

typedef struct SapbrDeactURC_s {
  int32_t id;
} SapbrDeactURC_t;

typedef struct SapbrURC_s {
  SapbrURC_Type_t type;
  union {
    SapbrDeactURC_t deact;
  } payload;
} SapbrURC_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void SapbrObjectInit(Sapbr_t *this);

void SapbrSetupRequestSetConnTypeGprs(Sapbr_t *this, int32_t id);

void SapbrSetupRequestSetApn(Sapbr_t *this, int32_t id, const char *apn);

void SapbrSetupRequestOpenContext(Sapbr_t *this, int32_t id);

void SapbrSetupRequestCloseContext(Sapbr_t *this, int32_t id);

AT_Command_t *SapbrGetAtCommand(Sapbr_t *this);

Sapbr_Response_t SapbrGetResponse(Sapbr_t *this);

AT_CommandStatus_t SapbrGetResponseStatus(Sapbr_t *this);

bool SapbrIsURC(const char *ibuf, size_t ilen);

size_t SapbrParseURC(SapbrURC_t *urc, const char *ibuf, size_t ilen);

#endif /* SAPBR_H */

/****************************** END OF FILE **********************************/
