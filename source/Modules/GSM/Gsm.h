/**
 * @file Gsm.h
 * @brief
 */

#ifndef GSM_H
#define GSM_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Call.h"
#include "Gprs.h"
#include "Sms.h"
#include "source/Modem/Modem.h"

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
  GSM_STATE_INVALID,
  GSM_STATE_INIT,
  GSM_STATE_WAIT_FOR_START,
  GSM_STATE_STARTED,
  GSM_STATE_STOPPED,
} GSM_State_t;

typedef struct Gsm_s {
  struct {
    uint32_t callready : 1;
    uint32_t smsready  : 1;
  } status;
  GSM_State_t state;
  GSM_Modem_t *modem;
  Call_t call;
  Gprs_t gprs;
  Sms_t sms;
} Gsm_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void GSM_GsmObjectInit(Gsm_t *this, GSM_Modem_t *modem);

bool GSM_GsmGprsRegisterCallback(Gsm_t *this, GPRS_EventCb_t cb);

bool GSM_GsmStart(Gsm_t *this, const char *pin);

bool GSM_GsmStop(Gsm_t *this);

bool GSM_GsmGprsStart(Gsm_t *this, const char *apn, const char *user, const char *passwd);

bool GSM_GsmGprsOpenTcpPort(Gsm_t *this, const char *host, int32_t port);

bool GSM_GsmGprsSend(Gsm_t *this, const char *data, size_t dlen);

bool GSM_GsmGprsCloseTcpPort(Gsm_t *this);

bool GSM_GsmGprsStop(Gsm_t *this);

size_t GSM_GsmURCParse(void *p, const char *ibuf, size_t ilen);


#endif /* GSM_H */

/****************************** END OF FILE **********************************/
