/**
 * @file GprsEvent.h
 * @brief
 */

#ifndef GPRS_EVENT_H
#define GPRS_EVENT_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stdint.h>
#include <stddef.h>

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
  GPRS_NO_EVENT,
  GPRS_CONNECT_OK,
  GPRS_CONNECT_FAIL,
  GPRS_CONNECTION_CLOSED,
  GPRS_ALREADY_CONNECTED,
  GPRS_CLOSED,
  GPRS_SEND_OK,
  GPRS_SEND_FAIL,
  GPRS_DISCONNECTED,
} GPRS_EventType_t;

typedef struct GPRS_ConnectFail_s {
  int32_t state;
} GPRS_ConnectFail_t;

typedef struct GPRS_Event_s {
  GPRS_EventType_t type;
  union {
    GPRS_ConnectFail_t fail;
  } payload;
} GPRS_Event_t;

typedef void (*GPRS_EventCb_t)(GPRS_Event_t *p);

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/

#endif /* GPRS_EVENT_H */

/****************************** END OF FILE **********************************/
