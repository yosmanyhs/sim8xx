/**
 * @file cstt.h
 * @brief
 */

#ifndef CSTT_H
#define CSTT_H

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
typedef struct Cstt_request_s {
  const char *apn;
  const char *user;
  const char *passwd;
} Cstt_request_t;

typedef struct Cstt_response_s {
  AT_CommandStatus_t status;
} Cstt_response_t;

typedef struct Cstt_s {
  Cstt_request_t request;
  Cstt_response_t response;
  AT_Command_t atcmd;
} Cstt_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void CsttObjectInit(Cstt_t *this);

void CsttSetupRequest(Cstt_t *this, const char *apn, const char *user, const char *passwd);

AT_Command_t *CsttGetAtCommand(Cstt_t *this);

Cstt_response_t CsttGetResponse(Cstt_t *this);

AT_CommandStatus_t CsttGetResponseStatus(Cstt_t *this);

#endif /* CSTT_H */

/****************************** END OF FILE **********************************/
