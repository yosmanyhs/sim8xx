/**
 * @file Sim8xx.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Interface/Os.h"
#include "Sim8xx.h"


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
void SIM_Init(Sim8xx_t *this, Sim8xxConfig_t *config)
{
  OS_Init();
  GSM_ModemObjectInit(&this->modem);
  GSM_ModemRegisterPutFunction(&this->modem, config->put);
  GSM_BufferObjectInit(&this->buffer);
}

bool SIM_Start(Sim8xx_t *this)
{
  bool result = false;

  if (GSM_ModemIsAlive(&this->modem)) {
    result = GSM_ModemDisableEcho(&this->modem);
  }

  return result;
}

bool SIM_Stop(Sim8xx_t *this)
{
  (void)this;
  return true;
}

bool SIM_RegisterBluetoothCallback(Sim8xx_t *this, GSM_BluetoothCb_t cb)
{
  return GSM_ModemRegisterBluetoothCallback(&this->modem, cb);
}

bool SIM_IsAlive(Sim8xx_t *this)
{
  return GSM_ModemIsAlive(&this->modem);
}

bool SIM_ProcessChar(Sim8xx_t *this, char c)
{
  return GSM_BufferPushChar(&this->buffer, c);
}

void SIM_Parse(Sim8xx_t *this)
{
  size_t msglen = 0;
  do {
    GSM_BufferData_t ibuf = GSM_BufferGetData(&this->buffer);
    msglen                = GSM_ModemParse(&this->modem, ibuf.data, ibuf.length);

    if (0 < msglen) {
      GSM_BufferPopData(&this->buffer, msglen);
    }
  } while (msglen && GSM_BufferGetLength(&this->buffer));
}

bool SIM_BluetoothSetup(Sim8xx_t *this, const char *name, const char *pin)
{
  return GSM_ModemBluetoothSetup(&this->modem, name, pin);
}

bool SIM_BluetoothStart(Sim8xx_t *this)
{
  return GSM_ModemBluetoothStart(&this->modem);
}

bool SIM_BluetoothStop(Sim8xx_t *this)
{
  return GSM_ModemBluetoothStop(&this->modem);
}

bool SIM_BluetoothAcceptConnection(Sim8xx_t *this)
{
  return GSM_ModemBluetoothAcceptConnection(&this->modem);
}

bool SIM_BluetoothSendSppData(Sim8xx_t *this, const char data[], size_t length)
{
  return GSM_ModemBluetoothSendSppData(&this->modem, data, length);
}

bool SIM_GpsStart(Sim8xx_t *this)
{
  return GSM_ModemGpsStart(&this->modem);
}

bool SIM_GpsStop(Sim8xx_t *this)
{
  return GSM_ModemGpsStop(&this->modem);
}

bool SIM_GpsReadPosition(Sim8xx_t *this, GPS_Data_t *data)
{
  return GSM_ModemGpsRead(&this->modem, data);
}

bool SIM_GsmGprsRegisterCallback(Sim8xx_t *this, GPRS_EventCb_t cb)
{
  return GSM_ModemGprsRegisterCallback(&this->modem, cb);
}

bool SIM_GsmStart(Sim8xx_t *this, const char *pin)
{
  return GSM_ModemStart(&this->modem, pin);
}

bool SIM_GsmStop(Sim8xx_t *this)
{
  return GSM_ModemStop(&this->modem);
}

bool SIM_GsmGprsStart(Sim8xx_t *this, const char *apn, const char *user, const char *passwd)
{
  return GSM_ModemGprsStart(&this->modem, apn, user, passwd);
}

bool SIM_GsmGprsOpenTcpPort(Sim8xx_t *this, const char *host, int32_t port)
{
  return GSM_ModemGprsOpenTcpPort(&this->modem, host, port);
}

bool SIM_GsmGprsSend(Sim8xx_t *this, const char *data, size_t dlen)
{
  return GSM_ModemGprsSend(&this->modem, data, dlen);
}

bool SIM_GsmGprsCloseTcpPort(Sim8xx_t *this)
{
  return GSM_ModemGprsCloseTcpPort(&this->modem);
}

bool SIM_GsmGprsStop(Sim8xx_t *this)
{
  return GSM_ModemGprsStop(&this->modem);
}

/****************************** END OF FILE **********************************/
