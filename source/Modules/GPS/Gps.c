/**
 * @file Gps.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Gps.h"
#include "Modem.h"
#include "cgnspwr.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

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

/****************************** END OF FILE **********************************/
