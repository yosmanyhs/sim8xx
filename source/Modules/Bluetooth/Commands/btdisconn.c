/**
 * @file Btdisconn.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "btdisconn.h"
#include "Common/Env.h"
#include "Utils/Utils.h"

#include <stdio.h>
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
GSM_STATIC bool BtDisconnIsBtDisconn(const char *ibuf, size_t ilen)
{
  const char *tag = "\r\n+BTDISCONN:";
  size_t tlen     = strlen(tag);

  bool result = false;
  if (tlen < ilen) {
    result = (0 == strncasecmp(ibuf, tag, tlen));
  }

  return result;
}

GSM_STATIC size_t BtDisconn_parseBtDisconn(BtDisconn_DisconnectedURC_t *urc,
                                           const char *ibuf,
                                           size_t ilen)
{
  size_t offset = 0;
  memset(urc, 0, sizeof(*urc));

  if (!BtDisconnIsBtDisconn(ibuf, ilen))
    return 0;

  const char *next = ibuf;
  const char *end  = ibuf + ilen;

  size_t n = GSM_UtilsGetString(next, end - next, urc->name, sizeof(urc->name), '"', '"');
  if (n) {
    offset += n + 1;
    next += n + 1;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  n = GSM_UtilsGetString(next, end - next, urc->address, sizeof(urc->address), ',', ',');
  if (n) {
    offset += n + 1;
    next += n + 1;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  n = GSM_UtilsGetString(next, end - next, urc->profile, sizeof(urc->profile), '"', '"');
  if (n) {
    offset += n;
    next += n + 1;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  if (0 == strncmp(next, "\r\n", 2)) {
    offset += 2 + 1;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  return offset;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
bool BtDisconnIsURC(const char *ibuf, size_t ilen)
{
  return BtDisconnIsBtDisconn(ibuf, ilen);
}

size_t BtDisconnParseURC(BtDisconnURC_t *urc, const char *ibuf, size_t ilen)
{
  size_t offset = 0;

  if (BtDisconnIsBtDisconn(ibuf, ilen)) {
    urc->type = BTDISCONN_DISCONNECTED;
    offset    = BtDisconn_parseBtDisconn(&urc->payload.disconn, ibuf, ilen);
  } else {
    urc->type = BTDISCONN_NO_URC;
  }

  if (0 == offset)
    urc->type = BTDISCONN_NO_URC;

  return offset;
}

/****************************** END OF FILE **********************************/
