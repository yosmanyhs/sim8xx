/**
 * @file Sim8xx.h
 * @brief
 */

#ifndef SIM8XX_H
#define SIM8XX_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Buffer/Buffer.h"
#include "Modem/Modem.h"
#include "Modules/Bluetooth/Bluetooth.h"
#include "Modules/GPS/Gps.h"

#include <stdbool.h>
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
typedef struct Sim8xxConfig_s {
  GSM_SerialPut_t put;
} Sim8xxConfig_t;

typedef struct Sim8xx_s {
  GSM_Modem_t modem;
  GSM_Buffer_t buffer;
} Sim8xx_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void SIM_Init(Sim8xx_t *this, Sim8xxConfig_t *config);

bool SIM_Start(Sim8xx_t *this);

bool SIM_Stop(Sim8xx_t *this);

bool SIM_RegisterModemCallback(Sim8xx_t *this, GSM_ModemCb_t cb);

bool SIM_RegisterBluetoothCallback(Sim8xx_t *this, GSM_BluetoothCb_t cb);

bool SIM_RegisterIpCallback(Sim8xx_t *this, GSM_IpCb_t cb);

bool SIM_IsAlive(Sim8xx_t *this);

bool SIM_ProcessChar(Sim8xx_t *this, char c);

bool SIM_UnlockSIMCard(Sim8xx_t *this, const char *pin);

double SIM_GetSignalStrength(Sim8xx_t *this);

void SIM_Parse(Sim8xx_t *this);

bool SIM_BluetoothSetup(Sim8xx_t *this, const char *name, const char *pin);

bool SIM_BluetoothStart(Sim8xx_t *this);

bool SIM_BluetoothStop(Sim8xx_t *this);

bool SIM_BluetoothAcceptConnection(Sim8xx_t *this);

bool SIM_BluetoothSendSppData(Sim8xx_t *this, const char data[], size_t length);

bool SIM_GpsStart(Sim8xx_t *this);

bool SIM_GpsStop(Sim8xx_t *this);

bool SIM_GpsReadPosition(Sim8xx_t *this, GPS_Data_t *data);

bool SIM_IpSetup(Sim8xx_t *this, const char *apn);

bool SIM_IpOpen(Sim8xx_t *this);

bool SIM_IpClose(Sim8xx_t *this);

bool SIM_IpHttpStart(Sim8xx_t *this);

bool SIM_IpHttpGet(Sim8xx_t *this, const char *url);

bool SIM_IpHttpStop(Sim8xx_t *this);

#endif /* SIM8XX_H */

/****************************** END OF FILE **********************************/
