/**
 * @file cgnspwr.h
 * @brief
 */

#ifndef CGNS_PWR_H
#define CGNS_PWR_H

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
typedef struct CgnsPwr_request_s {
  uint8_t mode;
} CgnsPwr_request_t;

typedef struct CgnsPwr_response_s {
  AT_CommandStatus_t status;
} CgnsPwr_response_t;

typedef struct CgnsPwr_s {
  CgnsPwr_request_t request;
  CgnsPwr_response_t response;
  AT_Command_t atcmd;
} CgnsPwr_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void CgnsPwrObjectInit(CgnsPwr_t *this);

void CgnsPwrSetupRequest(CgnsPwr_t *this, uint8_t mode);

AT_Command_t *CgnsPwrGetAtCommand(CgnsPwr_t *this);

CgnsPwr_response_t CgnsPwrGetResponse(CgnsPwr_t *this);

AT_CommandStatus_t CgnsPwrGetResponseStatus(CgnsPwr_t *this);

#endif /* CGNS_PWR_H */

/****************************** END OF FILE **********************************/
