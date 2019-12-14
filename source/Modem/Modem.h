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
typedef struct GSM_Modem_s {
    AT_Command_t *currentAt;
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

bool GSM_ModemRegisterBluetoothCallback(GSM_Modem_t *this, GSM_BluetoothCb_t cb);

void GSM_ModemExecuteAtCommand(GSM_Modem_t *this, AT_Command_t *atcmd);

size_t GSM_ModemParse(GSM_Modem_t *this, const char *ibuf, size_t ilen);

#endif /* MODEM_H */

/****************************** END OF FILE **********************************/
