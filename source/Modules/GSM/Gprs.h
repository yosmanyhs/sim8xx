/**
 * @file Gprs.h
 * @brief
 */

#ifndef GPRS_H
#define GPRS_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Modules/GSM/GprsEvent.h"
#include <stdbool.h>

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
  GPRS_STATE_STOPPED,
  GPRS_STATE_DISCONNECTED,
  GPRS_STATE_WAIT_FOR_CONNECT,
  GPRS_STATE_CONNECTED,
} GPRS_State_t;

typedef struct GSM_Modem_s GSM_Modem_t;

typedef struct Gprs_s {
  GSM_Modem_t *modem;
  GPRS_EventCb_t notify;
  GPRS_State_t state;
  char ipaddr[4 * 3 + 3 + 1];
  GPRS_Event_t event;
} Gprs_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void GprsObjectInit(Gprs_t *this, GSM_Modem_t *modem);

bool GprsRegisterEventCallback(Gprs_t *this, GPRS_EventCb_t cb);

bool GprsStart(Gprs_t *this, const char *apn, const char *user, const char *passwd);

bool GprsOpenTcpPort(Gprs_t *this, const char *host, int32_t port);

bool GprsSend(Gprs_t *this, const char *data, size_t dlen);

bool GprsCloseTcpPort(Gprs_t *this);

bool GprsStop(Gprs_t *this);

size_t GprsURCParse(void *p, const char *ibuf, size_t ilen);

#endif /* GPRS_H */

/****************************** END OF FILE **********************************/
