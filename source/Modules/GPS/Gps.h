/**
 * @file Gps.h
 * @brief
 */

#ifndef GPS_H
#define GPS_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "GpsData.h" 

#include <stdbool.h>
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
typedef struct GSM_Modem_s GSM_Modem_t;
typedef struct GSM_Gps_s {
    GSM_Modem_t *parent;
} GSM_Gps_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void GSM_GpsObjectInit(GSM_Gps_t *this, GSM_Modem_t *parent);

bool GSM_GpsStart(GSM_Gps_t *this);

bool GSM_GpsStop(GSM_Gps_t *this);

bool GSM_GpsRead(GSM_Gps_t *this, GPS_Data_t *data);

#endif /* GPS_H */

/****************************** END OF FILE **********************************/