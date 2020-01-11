/**
 * @file atcpin.h
 * @brief
 */

#ifndef ATCPIN_H
#define ATCPIN_H

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
  ATCPIN_NO_URC,
  ATCPIN_INFO,
} AtCpinURC_Type_t;

typedef enum {
  ATCPIN_INVALID_CODE,
  ATCPIN_READY,
  ATCPIN_SIM_PIN,
  ATCPIN_SIM_PUK,
  ATCPIN_PH_SIM_PIN,
  ATCPIN_PH_SIM_PUK,
  ATCPIN_SIM_PIN2,
  ATCPIN_SIM_PUK2,
} AtCpinCode_t;

typedef struct AtCpinInfo_s {
  AtCpinCode_t code;
} AtCpinInfo_t;

typedef struct AtCpinURC_s {
  AtCpinURC_Type_t type;
  union {
    AtCpinInfo_t info;
  } payload;
} AtCpinURC_t;

typedef struct AtCpin_Request_s {
  const char *pin;
  const char *pin2;
} AtCpin_Request_t;

typedef struct AtCpin_Response_s {
  AT_CommandStatus_t status;
} AtCpin_Response_t;

typedef struct AtCpin_s {
  AtCpin_Request_t request;
  AtCpin_Response_t response;
  AT_Command_t atcmd;
} AtCpin_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void AtCpinObjectInit(AtCpin_t *this);

void AtCpinSetupRequest(AtCpin_t *this, const char *pin, const char *pin2);

AT_Command_t *AtCpinGetAtCommand(AtCpin_t *this);

AtCpin_Response_t AtCpinGetResponse(AtCpin_t *this);

AT_CommandStatus_t AtCpinGetResponseStatus(AtCpin_t *this);

bool AtCpinIsURC(const char *ibuf, size_t ilen);

size_t AtCpinParseURC(AtCpinURC_t *urc, const char *ibuf, size_t ilen);

#endif /* ATCPIN_H */

/****************************** END OF FILE **********************************/
