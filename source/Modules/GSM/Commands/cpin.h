/**
 * @file atcpin.h
 * @brief
 */

#ifndef CPIN_H
#define CPIN_H

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
  CPIN_NO_URC,
  CPIN_INFO,
} CpinURC_Type_t;

typedef enum {
  CPIN_INVALID_CODE,
  CPIN_NOT_INSERTED,
  CPIN_READY,
  CPIN_SIM_PIN,
  CPIN_SIM_PUK,
  CPIN_PH_SIM_PIN,
  CPIN_PH_SIM_PUK,
  CPIN_SIM_PIN2,
  CPIN_SIM_PUK2,
} CpinCode_t;

typedef struct CpinInfo_s {
  CpinCode_t code;
} CpinInfo_t;

typedef struct CpinURC_s {
  CpinURC_Type_t type;
  union {
    CpinInfo_t info;
  } payload;
} CpinURC_t;

typedef struct Cpin_Request_s {
  const char *pin;
  const char *pin2;
} Cpin_Request_t;

typedef struct Cpin_Response_s {
  AT_CommandStatus_t status;
} Cpin_Response_t;

typedef struct Cpin_s {
  Cpin_Request_t request;
  Cpin_Response_t response;
  AT_Command_t atcmd;
} Cpin_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void CpinObjectInit(Cpin_t *this);

void CpinSetupRequest(Cpin_t *this, const char *pin, const char *pin2);

AT_Command_t *CpinGetAtCommand(Cpin_t *this);

Cpin_Response_t CpinGetResponse(Cpin_t *this);

AT_CommandStatus_t CpinGetResponseStatus(Cpin_t *this);

bool CpinIsURC(const char *ibuf, size_t ilen);

size_t CpinParseURC(CpinURC_t *urc, const char *ibuf, size_t ilen);

#endif /* CPIN_H */

/****************************** END OF FILE **********************************/
