/**
 * @file Bluetooth.h
 * @brief
 */

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "Module.h"

#include "btconnect.h"

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

typedef void (*GSM_BluetoothCb)(GSM_BluetoothEvent_t *p);

typedef struct GSM_Bluetooth_s {
  GSM_Module_t module;
  GSM_BluetoothCb notify;
  GSM_BluetoothEvent_t event;
} GSM_Bluetooth_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void GSM_BluetoothObjectInit(GSM_Bluetooth_t *this);

bool GSM_BluetoothRegisterCallback(GSM_Bluetooth_t *this, GSM_BluetoothCb cb);

bool GSM_BluetoothSetup(GSM_Bluetooth_t *this, const char *name, const char *pin);

bool GSM_BluetoothStart(GSM_Bluetooth_t *this);

bool GSM_BluetoothStop(GSM_Bluetooth_t *this);

bool GSM_BluetoothAcceptConnection(GSM_Bluetooth_t *this);

bool GSM_BluetoothSendSPPData(GSM_Bluetooth_t *this, const char *data, size_t length);

size_t GSM_BluetoothURCParse(void *p, const char *ibuf, size_t length);

#endif /* BLUETOOTH_H */

/****************************** END OF FILE **********************************/