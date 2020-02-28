/**
 * @file Ip.h
 * @brief
 */

#ifndef IP_H
#define IP_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "IpEvents.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

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

typedef struct GSM_Ip_s {
  GSM_Modem_t *modem;
  GSM_IpCb_t notify;
  GSM_IpEvent_t event;
} GSM_Ip_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void GSM_IpObjectInit(GSM_Ip_t *this, GSM_Modem_t *modem);

bool GSM_IpRegisterCallback(GSM_Ip_t *this, GSM_IpCb_t cb);

bool GSM_IpSetup(GSM_Ip_t *this, const char *apn);

bool GSM_IpOpen(GSM_Ip_t *this);

bool GSM_IpClose(GSM_Ip_t *this);

size_t GSM_IpURCParse(void *p, const char *ibuf, size_t ilen);

bool GSM_IpHttpStart(GSM_Ip_t *this);

bool GSM_IpHttpGet(GSM_Ip_t *this, const char *url);

bool GSM_IpHttpStop(GSM_Ip_t *this);

#endif /* IP_H */

/****************************** END OF FILE **********************************/
