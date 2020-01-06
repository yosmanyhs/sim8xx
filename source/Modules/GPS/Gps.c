/**
 * @file Gps.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Gps.h"
#include "Common/Env.h"
#include "Modem/Modem.h"
#include "Commands/cgnspwr.h"
#include "Commands/cgnsinf.h"

#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/
#define NUM(n) ((n) - '0')

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
GSM_STATIC bool GSM_gpsConvertRawDateTimeToGpsDateTime(GPS_Time_t *time, char *ibuf, size_t ilen)
{
  if (18 != ilen)
    return false;

  time->year  = 1000 * NUM(ibuf[0]);
  time->year += 100  * NUM(ibuf[1]);
  time->year += 10   * NUM(ibuf[2]);
  time->year += 1    * NUM(ibuf[3]);

  time->month  = 10 * NUM(ibuf[4]);
  time->month += 1  * NUM(ibuf[5]);

  time->day  = 10 * NUM(ibuf[6]);
  time->day += 1  * NUM(ibuf[7]);

  time->hour  = 10 * NUM(ibuf[8]);
  time->hour += 1  * NUM(ibuf[9]);

  time->min  = 10 * NUM(ibuf[10]);
  time->min += 1  * NUM(ibuf[11]);

  time->sec  = 10 * NUM(ibuf[12]);
  time->sec += 1  * NUM(ibuf[13]);

  time->msec  = 100 * NUM(ibuf[15]);
  time->msec += 10  * NUM(ibuf[16]);
  time->msec += 1   * NUM(ibuf[17]);

  return true;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void GSM_GpsObjectInit(GSM_Gps_t *this, GSM_Modem_t *parent)
{
    this->parent = parent;
}

bool GSM_GpsStart(GSM_Gps_t *this)
{
  CgnsPwr_t cgnspwr = {0};
  CgnsPwrObjectInit(&cgnspwr);
  CgnsPwrSetupRequest(&cgnspwr, 1);

  GSM_ModemExecuteAtCommand(this->parent, &cgnspwr.atcmd);

  return AT_CMD_OK == CgnsPwrGetResponseStatus(&cgnspwr);
}

bool GSM_GpsStop(GSM_Gps_t *this)
{
  CgnsPwr_t cgnspwr = {0};
  CgnsPwrObjectInit(&cgnspwr);
  CgnsPwrSetupRequest(&cgnspwr, 0);

  GSM_ModemExecuteAtCommand(this->parent, &cgnspwr.atcmd);

  return AT_CMD_OK == CgnsPwrGetResponseStatus(&cgnspwr);
}

bool GSM_GpsRead(GSM_Gps_t *this, GPS_Data_t *data)
{
  CgnsInf_t cgnsinf;
  CgnsInfObjectInit(&cgnsinf);

  GSM_ModemExecuteAtCommand(this->parent, &cgnsinf.atcmd);

  bool result = false;
  if (AT_CMD_OK == CgnsInfGetResponseStatus(&cgnsinf)) {
    char *date = cgnsinf.response.date;
    size_t datelength = strlen(date);

    if (GSM_gpsConvertRawDateTimeToGpsDateTime(&data->time, date, datelength)) {
      data->isLocked = (1 == cgnsinf.response.fixStatus);
      data->latitude = cgnsinf.response.latitude;
      data->longitude = cgnsinf.response.longitude;
      data->altitude = cgnsinf.response.altitude;
      data->speed = cgnsinf.response.speed;
      data->gpsSatInView = cgnsinf.response.gpsSatInView;
      data->gnssSatInUse = cgnsinf.response.gnssSatInUse;
      data->gnssSatInView = cgnsinf.response.gnssSatInView;   
      result = true;
    }
  }

  return result;
}

/****************************** END OF FILE **********************************/
