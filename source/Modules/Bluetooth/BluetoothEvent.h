/**
 * @file BluetoothEvent.h
 * @brief
 */

#ifndef BLUETOOTH_EVENT_H
#define BLUETOOTH_EVENT_H

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
  GSM_BT_NO_EVENT,
  GSM_BT_CONNECTING,
  GSM_BT_CONNECTED,
  GSM_BT_INCOMING_DATA,
  GSM_BT_DISCONNECTED,
} GSM_BluetoothEventType_t;

typedef struct GSM_BluetoothEvent_Connecting_s {
  char address[19];
  char profile[10];
} GSM_BluetoothEvent_Connecting_t;

typedef struct GSM_BluetoothEvent_Connect_s {
  uint32_t id;
  char name[19];
  char address[19];
  char profile[10];
} GSM_BluetoothEvent_Connected_t;

typedef struct GSM_BluetoothEvent_IncomingData_s {
  char data[256];
} GSM_BluetoothEvent_IncomingData_t;

typedef struct GSM_BluetoothEvent_Disconnected_s {
  char name[19];
} GSM_BluetoothEvent_Disconnected_t;

typedef struct GSM_BluetoothEvent_s {
  GSM_BluetoothEventType_t type;
  union {
    GSM_BluetoothEvent_Connecting_t connecting;
    GSM_BluetoothEvent_Connected_t connected;
    GSM_BluetoothEvent_IncomingData_t incomingData;
    GSM_BluetoothEvent_Disconnected_t disconnected;
  } payload;
} GSM_BluetoothEvent_t;

typedef void (*GSM_BluetoothCb_t)(GSM_BluetoothEvent_t *p);

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/

#endif /* BLUETOOTH_CALLBACK_H */

/****************************** END OF FILE **********************************/
