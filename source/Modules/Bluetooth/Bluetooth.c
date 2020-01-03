/**
 * @file Bluetooth.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Bluetooth.h"
#include "Env.h"

#include "Modem/Modem.h"
#include "Common/AtCommand.h"
#include "Commands/btacpt.h"
#include "Commands/btconnect.h"
#include "Commands/btdisconn.h"
#include "Commands/bthost.h"
#include "Commands/btpaircfg.h"
#include "Commands/btpower.h"
#include "Commands/btsppsend.h"
#include "Commands/btsppget.h"

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
GSM_STATIC void GSM_bluetoothHandleBtConnectURC(BtConnectURC_t *urc, GSM_Bluetooth_t *blt)
{
  switch(urc->type) {
    case BTCONNECT_CONNECTING: {
      blt->event.type = GSM_BT_CONNECTING;
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
      blt->event.type = GSM_BT_CONNECTED;
      blt->event.payload.connected.id = urc->payload.connect.id;
      const char *src = urc->payload.connect.name;
      char *dst = blt->event.payload.connected.name;
      size_t n = sizeof(blt->event.payload.connected.name);
      memcpy(dst, src, n);
      src = urc->payload.connect.address;
      dst = blt->event.payload.connected.address;
      n = sizeof(blt->event.payload.connected.address);
      memcpy(dst, src, n);
      src = urc->payload.connect.profile;
      dst = blt->event.payload.connected.profile;
      n = sizeof(blt->event.payload.connected.profile);
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

GSM_STATIC void GSM_bluetoothHandleBtSppGetURC(BtSppGetURC_t *urc, GSM_Bluetooth_t *blt)
{
  switch(urc->type) {
    case BTSPPGET_SPPDATA: {
      blt->event.type = GSM_BT_INCOMING_DATA;
      const char *src = urc->payload.getdata.data;
      char *dst = blt->event.payload.incomingData.data;
      size_t buflength = sizeof(blt->event.payload.incomingData.data);
      size_t length = urc->payload.getdata.length;
      length = (length < buflength) ? length : buflength;
      memcpy(dst, src, length);

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

GSM_STATIC void GSM_bluetoothHandleBtDisconnURC(BtDisconnURC_t *urc, GSM_Bluetooth_t *blt)
{
  switch(urc->type) {
    case BTDISCONN_DISCONNECTED: {
      blt->event.type = GSM_BT_DISCONNECTED;
      const char *src = urc->payload.disconn.name;
      char *dst = blt->event.payload.disconnected.name;
      size_t buflength = sizeof(blt->event.payload.disconnected.name);
      size_t length = sizeof(urc->payload.disconn.name);
      length = (length < buflength) ? length : buflength;
      memcpy(dst, src, length);

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
void GSM_BluetoothObjectInit(GSM_Bluetooth_t *this, GSM_Modem_t *parent)
{
  memset(&this->event, 0, sizeof(this->event));
  this->parent = parent;
  this->notify = NULL;
}

bool GSM_BluetoothRegisterCallback(GSM_Bluetooth_t *this, GSM_BluetoothCb_t cb)
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
  GSM_ModemExecuteAtCommand(this->parent, &btpaircfg.atcmd);

  if (AT_CMD_OK != BtPaircfgGetResponseStatus(&btpaircfg))
    return false;

  BtHost_t bthost = {0};
  BtHostObjectInit(&bthost);
  BtHostSetupRequest(&bthost, name);
  GSM_ModemExecuteAtCommand(this->parent, &bthost.atcmd);

  return AT_CMD_OK == BtHostGetResponseStatus(&bthost);
}

bool GSM_BluetoothStart(GSM_Bluetooth_t *this)
{
  BtPower_t btpower = {0};
  BtPowerObjectInit(&btpower);
  BtPowerSetupRequest(&btpower, 1);

  GSM_ModemExecuteAtCommand(this->parent, &btpower.atcmd);

  return (AT_CMD_OK == BtPowerGetResponseStatus(&btpower));
}

bool GSM_BluetoothStop(GSM_Bluetooth_t *this)
{
  BtPower_t btpower = {0};
  BtPowerObjectInit(&btpower);
  BtPowerSetupRequest(&btpower, 0);

  AT_Command_t *atcmd = BtPowerGetAtCommand(&btpower);
  GSM_ModemExecuteAtCommand(this->parent, atcmd);

  return (AT_CMD_OK == BtPowerGetResponseStatus(&btpower));
}

bool GSM_BluetoothAcceptConnection(GSM_Bluetooth_t *this)
{
  BtAcpt_t btacpt = {0};
  BtAcptObjectInit(&btacpt);
  BtAcptSetupRequest(&btacpt, 1);

  AT_Command_t *atcmd = BtAcptGetAtCommand(&btacpt);
  GSM_ModemExecuteAtCommand(this->parent, atcmd);

  return (AT_CMD_OK == BtAcptGetResponseStatus(&btacpt));
}

bool GSM_BluetoothSendSPPData(GSM_Bluetooth_t *this, const char *data, size_t length)
{
  BtSppSend_t btsppsend = {0};
  BtSppSendObjectInit(&btsppsend);
  BtSppSendSetupRequest(&btsppsend, data, length);
  BtSppSendSetCommandMode(&btsppsend);
  GSM_ModemExecuteAtCommand(this->parent, &btsppsend.atcmd);

  if (AT_CMD_WAIT_FOR_USER_DATA != BtSppSendGetResponseStatus(&btsppsend))
    return false;

  BtSppSendSetDataMode(&btsppsend);
  GSM_ModemExecuteAtCommand(this->parent, &btsppsend.atcmd);

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
  else if (BtSppGetIsURC(ibuf, length)) {
    BtSppGetURC_t urc = {0};
    offset = BtSppGetParseURC(&urc, ibuf, length);
    GSM_bluetoothHandleBtSppGetURC(&urc, blt);
  } 
  else if (BtDisconnIsURC(ibuf, length)) {
    BtDisconnURC_t urc = {0};
    offset = BtDisconnParseURC(&urc, ibuf, length);
    GSM_bluetoothHandleBtDisconnURC(&urc, blt);
  } else {
    ;
  }
  
  return offset;
}

/****************************** END OF FILE **********************************/
