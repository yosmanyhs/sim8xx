/**
 * @file Modem.h
 * @brief
 */

#ifndef MODEM_H
#define MODEM_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Common/AtCommand.h"
#include "Modules/Bluetooth/Bluetooth.h"
#include "Modules/GPS/Gps.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef void (*GSM_SerialPut_t)(char c);

typedef struct GSM_Modem_s {
  AT_Command_t *currentAt;
  GSM_SerialPut_t put;
  GSM_Bluetooth_t bluetooth;
  GSM_Gps_t gps;
} GSM_Modem_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void GSM_ModemObjectInit(GSM_Modem_t *this);

bool GSM_ModemRegisterPutFunction(GSM_Modem_t *this, GSM_SerialPut_t put);

bool GSM_ModemRegisterBluetoothCallback(GSM_Modem_t *this, GSM_BluetoothCb_t cb);

bool GSM_ModemIsAlive(GSM_Modem_t *this);

bool GSM_ModemDisableEcho(GSM_Modem_t *this);

void GSM_ModemExecuteAtCommand(GSM_Modem_t *this, AT_Command_t *atcmd);

size_t GSM_ModemParse(GSM_Modem_t *this, const char *ibuf, size_t ilen);

bool GSM_ModemBluetoothSetup(GSM_Modem_t *this, const char *name, const char *pin);

bool GSM_ModemBluetoothStart(GSM_Modem_t *this);

bool GSM_ModemBluetoothStop(GSM_Modem_t *this);

bool GSM_ModemBluetoothAcceptConnection(GSM_Modem_t *this);

bool GSM_ModemBluetoothSendSppData(GSM_Modem_t *this, const char data[], size_t length);

bool GSM_ModemGpsStart(GSM_Modem_t *this);

bool GSM_ModemGpsStop(GSM_Modem_t *this);

bool GSM_ModemGpsRead(GSM_Modem_t *this, GPS_Data_t *data);

#endif /* MODEM_H */

/****************************** END OF FILE **********************************/
