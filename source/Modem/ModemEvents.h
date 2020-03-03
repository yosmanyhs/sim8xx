/**
 * @file ModemEvents.h
 * @brief
 */

#ifndef MODEM_EVENT_H
#define MODEM_EVENT_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

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
  MODEM_NO_EVENT,
  MODEM_EVENT_SIM_UNLOCKED,
  MODEM_EVENT_CPIN,
} GSM_ModemEventType_t;

typedef enum {
  CPIN_INVALID_STATUS,
  CPIN_NOT_INSERTED,
  CPIN_READY,
  CPIN_PIN_REQUIRED,
  CPIN_PUK_REQUIRED,
} GSM_ModemCpinStatus_t;

typedef struct GSM_ModemEventCpin_s {
  GSM_ModemCpinStatus_t status;
} GSM_ModemEventCpin_t;

typedef struct ModemEvent_s {
  GSM_ModemEventType_t type;
  union {
    GSM_ModemEventCpin_t cpin;
  } payload;
} GSM_ModemEvent_t;

typedef void (*GSM_ModemCb_t)(GSM_ModemEvent_t *p);

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/

#endif /* MODEM_EVENT_H */

/****************************** END OF FILE **********************************/
