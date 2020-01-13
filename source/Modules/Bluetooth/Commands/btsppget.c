/**
 * @file btsppget.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "btsppget.h"
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
GSM_STATIC bool BtSppGetIsSppGet(const char *ibuf, size_t ilen)
{
  const char *tag = "\r\n+BTSPPDATA:";
  size_t tlen     = strlen(tag);

  bool result = false;
  if (tlen < ilen) {
    result = (0 == strncasecmp(ibuf, tag, tlen));
  }

  return result;
}

GSM_STATIC size_t BtSppGet_parseSppData(BtSppGetDataURC_t *urc, const char *ibuf, size_t ilen)
{
  size_t offset = 0;
  memset(urc, 0, sizeof(*urc));

  if (!BtSppGetIsURC(ibuf, ilen))
    return 0;

  const char *next = ibuf;
  const char *end  = ibuf + ilen;

  size_t n = GSM_UtilsGetInt(next, end - next, (int32_t *)&urc->id, ' ', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  n = GSM_UtilsGetInt(next, end - next, (int32_t *)&urc->length, ',', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  n = GSM_UtilsGetString(next, end - next, urc->data, sizeof(urc->data), ',', '\r');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  if (0 == strncmp(next, "\r\n", 2)) {
    offset += 2;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  return offset;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
bool BtSppGetIsURC(const char *ibuf, size_t ilen)
{
  return BtSppGetIsSppGet(ibuf, ilen);
}

size_t BtSppGetParseURC(BtSppGetURC_t *urc, const char *ibuf, size_t ilen)
{
  size_t offset = 0;

  if (BtSppGetIsSppGet(ibuf, ilen)) {
    urc->type = BTSPPGET_SPPDATA;
    offset    = BtSppGet_parseSppData(&urc->payload.getdata, ibuf, ilen);
  } else {
    urc->type = BTSPPGET_NO_URC;
  }

  if (0 == offset)
    urc->type = BTSPPGET_NO_URC;

  return offset;
}

/****************************** END OF FILE **********************************/
