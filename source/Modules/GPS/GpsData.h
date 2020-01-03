/**
 * @file GpsData.h
 * @brief
 */

#ifndef GPS_DATA_H
#define GPS_DATA_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
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
typedef struct GPS_Time_s {
  uint32_t year;
  uint32_t month;
  uint32_t day;
  uint32_t hour;
  uint32_t min;
  uint32_t sec;
  uint32_t msec;
} GPS_Time_t;

typedef struct GPS_Position_s {
  GPS_Time_t time;
  double latitude;
  double longitude;
  double altitude;
  double speed;
  int gpsSatInView;
  int gnssSatInUse;
  int gnssSatInView;
} GPS_Data_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/

#endif /* GPS_DATA_H */

/****************************** END OF FILE **********************************/