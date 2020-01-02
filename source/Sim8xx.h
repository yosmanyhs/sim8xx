/**
 * @file Sim8xx.h
 * @brief
 */

#ifndef SIM_8XX_H
#define SIM_8XX_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stdbool.h>
#include <stddef.h>

#include "Buffer/Buffer.h" 
#include "Modem/Modem.h"
#include "Modules/Bluetooth/BluetoothEvent.h"

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

bool SIM_RegisterBluetoothCallback(Sim8xx_t *this, GSM_BluetoothCb_t cb);

bool SIM_IsAlive(Sim8xx_t *this);

bool SIM_ProcessChar(Sim8xx_t *this, char c);

void SIM_Parse(Sim8xx_t *this);

#endif /* SIM_8_XX_H */

/****************************** END OF FILE **********************************/
