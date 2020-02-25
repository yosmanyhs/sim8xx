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

bool SIM_RegisterBluetoothCallback(Sim8xx_t *this, GSM_BluetoothCb_t cb);

bool SIM_IsAlive(Sim8xx_t *this);

bool SIM_ProcessChar(Sim8xx_t *this, char c);

void SIM_Parse(Sim8xx_t *this);

bool SIM_BluetoothSetup(Sim8xx_t *this, const char *name, const char *pin);

bool SIM_BluetoothStart(Sim8xx_t *this);

bool SIM_BluetoothStop(Sim8xx_t *this);

bool SIM_BluetoothAcceptConnection(Sim8xx_t *this);

bool SIM_BluetoothSendSppData(Sim8xx_t *this, const char data[], size_t length);

bool SIM_GpsStart(Sim8xx_t *this);

bool SIM_GpsStop(Sim8xx_t *this);

bool SIM_GpsReadPosition(Sim8xx_t *this, GPS_Data_t *data);

bool SIM_GsmGprsRegisterCallback(Sim8xx_t *this, GPRS_EventCb_t cb);

bool SIM_GsmStart(Sim8xx_t *this, const char *pin);

bool SIM_GsmStop(Sim8xx_t *this);

bool SIM_GsmGprsStart(Sim8xx_t *this, const char *apn, const char *user, const char *passwd);

bool SIM_GsmGprsOpenTcpPort(Sim8xx_t *this, const char *host, int32_t port);

bool SIM_GsmGprsSend(Sim8xx_t *this, const char *data, size_t dlen);

bool SIM_GsmGprsCloseTcpPort(Sim8xx_t *this);

bool SIM_GsmGprsStop(Sim8xx_t *this);

#endif /* SIM8XX_H */

/****************************** END OF FILE **********************************/
