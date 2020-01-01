/**
 * @file Modem.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Modem.h"

#include "Interface/Os.h"

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
void GSM_ModemObjectInit(GSM_Modem_t *this)
{
  this->currentAt = NULL;
  this->put       = NULL;
  GSM_BluetoothObjectInit(&this->bluetooth, this);
  GSM_GpsObjectInit(&this->gps, this);
}

bool GSM_ModemRegisterPutFunction(GSM_Modem_t *this, GSM_SerialPut_t put)
{
  bool result = false;

  if (!this->put) {
    this->put = put;
    result    = true;
  }

  return result;
}

bool GSM_ModemRegisterBluetoothCallback(GSM_Modem_t *this, GSM_BluetoothCb_t cb)
{
  return GSM_BluetoothRegisterCallback(&this->bluetooth, cb);
}

void GSM_ModemExecuteAtCommand(GSM_Modem_t *this, AT_Command_t *atcmd)
{
  OS_LockModem();

  char obuf[128] = {0};
  size_t olen    = sizeof(obuf);
  size_t n       = atcmd->serialize(atcmd->obj, obuf, olen);

  size_t i;
  for (i = 0; i < n; ++i) {
    this->put(obuf[i]);
  }

  this->put('\r');

  OS_LockParser();
  this->currentAt = atcmd;
  OS_UnlockParser();

  OS_WaitForMessageWithTimeout(atcmd->timeout);

  OS_LockParser();
  this->currentAt = NULL;
  OS_UnlockParser();

  OS_UnlockModem();
}

size_t GSM_ModemParse(GSM_Modem_t *this, const char *ibuf, size_t ilen)
{
  OS_LockParser();

  size_t offset = 0;
  if (this->currentAt) {
    AT_Parse_t parser = this->currentAt->parse;
    offset            = parser(this->currentAt->obj, ibuf, ilen);
    if (offset) {
      OS_WakeUpThreadWaitingForMessage();
    }
  }

  if (0 == offset) {
    offset = GSM_BluetoothURCParse(&this->bluetooth, ibuf, ilen);
  }

  OS_UnlockParser();

  return offset;
}

/****************************** END OF FILE **********************************/
