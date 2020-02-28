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
#include "Modules/IP/Ip.h"
#include "ModemEvents.h"

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
  struct {
    uint32_t ready     : 1;
    uint32_t callready : 1;
    uint32_t smsready  : 1;
  } status;
  GSM_ModemCb_t notify;
  GSM_ModemEvent_t event;
  GSM_Bluetooth_t bluetooth;
  GSM_Gps_t gps;
  GSM_Ip_t ip;
} GSM_Modem_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void GSM_ModemObjectInit(GSM_Modem_t *this);

bool GSM_ModemRegisterPutFunction(GSM_Modem_t *this, GSM_SerialPut_t put);

bool GSM_ModemRegisterCallback(GSM_Modem_t *this, GSM_ModemCb_t cb);

bool GSM_ModemRegisterBluetoothCallback(GSM_Modem_t *this, GSM_BluetoothCb_t cb);

bool GSM_ModemRegisterIpCallback(GSM_Modem_t *this, GSM_IpCb_t cb);

bool GSM_ModemIsAlive(GSM_Modem_t *this);

bool GSM_ModemDisableEcho(GSM_Modem_t *this);

bool GSM_ModemUnlockSIMCard(GSM_Modem_t *this, const char *pin);

void GSM_ModemLock(GSM_Modem_t *this);

void GSM_ModemUnlock(GSM_Modem_t *this);

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

bool GSM_ModemIpSetup(GSM_Modem_t *this, const char *apn);

bool GSM_ModemIpOpen(GSM_Modem_t *this);

bool GSM_ModemIpClose(GSM_Modem_t *this);

bool GSM_ModemIpHttpStart(GSM_Modem_t *this);

bool GSM_ModemIpHttpGet(GSM_Modem_t *this, const char *url);

bool GSM_ModemIpHttpStop(GSM_Modem_t *this);

#endif /* MODEM_H */

/****************************** END OF FILE **********************************/
