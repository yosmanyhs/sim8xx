/**
 * @file Bluetooth.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Bluetooth.h"

#include "At.h"
#include "Modem.h"
#include "btconnect.h"
#include "bthost.h"
#include "btpaircfg.h"
#include "btpower.h"
#include "btsppsend.h"

#include <stddef.h>
#include <stdint.h>
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

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
#if !defined(TEST)
static
#endif
void GSM_bluetoothHandleBtConnectURC(BtConnectURC_t *urc, GSM_Bluetooth_t *blt)
{
  switch(urc->type) {
    case BTCONNECT_CONNECTING: {
      blt->event.type = GSM_BT_BTCONNECTING;
      const char *src = urc->payload.connecting.address;
      char *dst = blt->event.payload.connecting.address;
      size_t n = sizeof(blt->event.payload.connecting.address);
      memcpy(dst, src, n);
      src = urc->payload.connecting.profile;
      dst = blt->event.payload.connecting.profile;
      n = sizeof(blt->event.payload.connecting.profile);
      memcpy(dst, src, n);
      if (blt->notify) {
        blt->notify(&blt->event);
      }
      break;
    }
    case BTCONNECT_CONNECT: {
      blt->event.type = GSM_BT_BTCONNECT;
      blt->event.payload.connect.id = urc->payload.connect.id;
      const char *src = urc->payload.connect.name;
      char *dst = blt->event.payload.connect.name;
      size_t n = sizeof(blt->event.payload.connect.name);
      memcpy(dst, src, n);
      src = urc->payload.connect.address;
      dst = blt->event.payload.connect.address;
      n = sizeof(blt->event.payload.connect.address);
      memcpy(dst, src, n);
      src = urc->payload.connect.profile;
      dst = blt->event.payload.connect.profile;
      n = sizeof(blt->event.payload.connect.profile);
      memcpy(dst, src, n);
      if (blt->notify) {
        blt->notify(&blt->event);
      }
      break;
    }
    default: {
      break;
    }
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void GSM_BluetoothObjectInit(GSM_Bluetooth_t *this)
{
  memset(this, 0, sizeof(*this));
  this->module.urcparse = GSM_BluetoothURCParse;
  this->notify = NULL;
}

bool GSM_BluetoothRegisterCallback(GSM_Bluetooth_t *this, GSM_BluetoothCb cb)
{
  bool result = false;
  if (!this->notify) {
    this->notify = cb;
    result       = true;
  }

  return result;
}

bool GSM_BluetoothSetup(GSM_Bluetooth_t *this, const char *name, const char *pin)
{
  BtPaircfg_t btpaircfg = {0};
  BtPaircfgObjectInit(&btpaircfg);
  BtPaircfgSetupRequest(&btpaircfg, 1, pin);
  GSM_ModemExecuteAtCommand(&btpaircfg.atcmd);

  if (AT_CMD_OK != BtPaircfgGetResponseStatus(&btpaircfg))
    return false;

  BtHost_t bthost = {0};
  BtHostObjectInit(&bthost);
  BtHostSetupRequest(&bthost, name);
  GSM_ModemExecuteAtCommand(&bthost.atcmd);

  return AT_CMD_OK == BtHostGetResponseStatus(&bthost);
}

bool GSM_BluetoothStart(GSM_Bluetooth_t *this)
{
  BtPower_t btpower = {0};
  BtPowerObjectInit(&btpower);
  BtPowerSetupRequest(&btpower, 1);

  GSM_ModemExecuteAtCommand(&btpower.atcmd);

  return (AT_CMD_OK == BtPowerGetResponseStatus(&btpower));
}

bool GSM_BluetoothStop(GSM_Bluetooth_t *this)
{
  BtPower_t btpower = {0};
  BtPowerObjectInit(&btpower);
  BtPowerSetupRequest(&btpower, 0);

  AT_Command_t *atcmd = BtPowerGetAtCommand(&btpower);
  GSM_ModemExecuteAtCommand(atcmd);

  return (AT_CMD_OK == BtPowerGetResponseStatus(&btpower));
}

bool GSM_BluetoothSendSPPData(GSM_Bluetooth_t *this, const char *data, size_t length)
{
  BtSppSend_t btsppsend = {0};
  BtSppSendObjectInit(&btsppsend);
  BtSppSendSetupRequest(&btsppsend, data, length);
  BtSppSendSetCommandMode(&btsppsend);
  GSM_ModemExecuteAtCommand(&btsppsend.atcmd);

  if (AT_CMD_WAIT_FOR_USER_DATA != BtSppSendGetResponseStatus(&btsppsend))
    return false;

  BtSppSendSetDataMode(&btsppsend);
  GSM_ModemExecuteAtCommand(&btsppsend.atcmd);

  return (AT_CMD_SEND_OK == BtSppSendGetResponseStatus(&btsppsend));
}

size_t GSM_BluetoothURCParse(void *p, const char *ibuf, size_t length)
{
  GSM_Bluetooth_t *blt = (GSM_Bluetooth_t *)p;
  size_t offset = 0;

  if (BtConnectIsURC(ibuf, length)) {
    BtConnectURC_t urc = {0};
    offset = BtConnectParseURC(&urc, ibuf, length);
    GSM_bluetoothHandleBtConnectURC(&urc, blt);
  }
  
  return offset;
}

/****************************** END OF FILE **********************************/
