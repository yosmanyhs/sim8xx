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

#include "BluetoothEvent.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef struct GSM_Modem_s GSM_Modem_t;
typedef struct GSM_Bluetooth_s {
  GSM_Modem_t *parent;
  GSM_BluetoothCb_t notify;
  GSM_BluetoothEvent_t event;
} GSM_Bluetooth_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void GSM_BluetoothObjectInit(GSM_Bluetooth_t *this, GSM_Modem_t *parent);

bool GSM_BluetoothRegisterCallback(GSM_Bluetooth_t *this, GSM_BluetoothCb_t cb);

bool GSM_BluetoothSetup(GSM_Bluetooth_t *this, const char *name, const char *pin);

bool GSM_BluetoothStart(GSM_Bluetooth_t *this);

bool GSM_BluetoothStop(GSM_Bluetooth_t *this);

bool GSM_BluetoothAcceptConnection(GSM_Bluetooth_t *this);

bool GSM_BluetoothSendSPPData(GSM_Bluetooth_t *this, const char *data, size_t length);

size_t GSM_BluetoothURCParse(void *p, const char *ibuf, size_t length);

#endif /* BLUETOOTH_H */

/****************************** END OF FILE **********************************/