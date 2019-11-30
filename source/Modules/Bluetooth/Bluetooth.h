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
  GSM_BT_SPP_DATA_RECEIVED,
  GSM_BT_SPP_DISCONNECTED,
} GSM_BluetoothEventId_t;

typedef struct GSM_BluetoothEvent_s {
  GSM_BluetoothEventId_t id;
  union {
    struct {
      uint8_t id;
      uint32_t length;
      char data[128];
    } spp;
  } data;
} GSM_BluetoothEvent_t;

typedef void (*GSM_BluetoothCb)(GSM_BluetoothEvent_t evt, void *p);

typedef struct GSM_Bluetooth_s {
  GSM_BluetoothCb notify;
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

bool GSM_BluetoothSendSPPData(GSM_Bluetooth_t *this, const char *data, size_t length);

#endif /* BLUETOOTH_H */

/****************************** END OF FILE **********************************/