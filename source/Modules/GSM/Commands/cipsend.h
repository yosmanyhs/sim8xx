/**
 * @file cipsend.h
 * @brief
 */

#ifndef CIPSEND_H
#define CIPSEND_H

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
typedef struct Cipsend_Request_s {
  const char *data;
  size_t length;
} Cipsend_Request_t;

typedef struct Cipsend_Response_s {
  AT_CommandStatus_t status;
} Cipsend_Response_t;

typedef enum {
  CIPSEND_COMMAND,
  CIPSEND_DATA,
} CipsendMode_t;

typedef struct Cipsend_s {
  Cipsend_Request_t request;
  Cipsend_Response_t response;
  CipsendMode_t mode;
  AT_Command_t atcmd;
} Cipsend_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void CipsendObjectInit(Cipsend_t *this);

void CipsendSetCommandMode(Cipsend_t *this);

void CipsendSetDataMode(Cipsend_t *this);

void CipsendSetupRequest(Cipsend_t *this, const char *data, size_t length);

AT_Command_t *CipsendGetAtCommand(Cipsend_t *this);

Cipsend_Response_t CipsendGetResponse(Cipsend_t *this);

AT_CommandStatus_t CipsendGetResponseStatus(Cipsend_t *this);

#endif /* CIPSEND_H */

/****************************** END OF FILE **********************************/
