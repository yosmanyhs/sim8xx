/**
 * @file httppara.h
 * @brief
 */

#ifndef HTTPPARA_H
#define HTTPPARA_H

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
typedef struct Httppara_Request_s {
  const char *key;
  const char *value;
} Httppara_Request_t;

typedef struct Httppara_Response_s {
  AT_CommandStatus_t status;
} Httppara_Response_t;

typedef struct Httppara_s {
  Httppara_Request_t request;
  Httppara_Response_t response;
  AT_Command_t atcmd;
} Httppara_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void HttpparaObjectInit(Httppara_t *this);

void HttpparaSetupRequestSetCid(Httppara_t *this, const char *cid);

void HttpparaSetupRequestSetUrl(Httppara_t *this, const char *url);

AT_Command_t *HttpparaGetAtCommand(Httppara_t *this);

Httppara_Response_t HttpparaGetResponse(Httppara_t *this);

AT_CommandStatus_t HttpparaGetResponseStatus(Httppara_t *this);

#endif /* HTTPPARA_H */

/****************************** END OF FILE **********************************/
