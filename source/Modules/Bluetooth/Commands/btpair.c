/**
 * @file btpair.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "btpair.h"

#include "Common/Env.h"
#include "Utils/Utils.h"

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
GSM_STATIC bool BtPairIsBtPairPassiveSuccess(const char *ibuf, size_t length)
{
  (void)length;
  const char *tag = "\r\n+BTPAIR:";
  return (0 == strncasecmp(ibuf, tag, strlen(tag)));
}

GSM_STATIC size_t BtPair_parsePassiveSuccessUrc(BtPairPassiveSuccess_t *urc, const char *ibuf, size_t length)
{
  size_t offset = 0;
  memset(urc, 0, sizeof(*urc));
  
  const char *next = ibuf;
  const char *end  = ibuf + length;

  size_t n = GSM_UtilsGetInt(next, end - next, &urc->id, ' ', ',');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  n = GSM_UtilsGetString(next, end - next, urc->name, sizeof(urc->name), '"', '"');
  if (n) {
    offset += n;
    next += n;
  } else {
    memset(urc, 0, sizeof(*urc));
    return 0;
  }

  n = GSM_UtilsGetString(next, end - next, urc->address, sizeof(urc->address), ',', '\r');
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
bool BtPairIsURC(const char *ibuf, size_t length)
{
  return BtPairIsBtPairPassiveSuccess(ibuf, length);
}

size_t BtPairParseURC(BtPairURC_t *urc, const char *ibuf, size_t length)
{
  size_t offset = 0;

  if (BtPairIsBtPairPassiveSuccess(ibuf, length)) {
    urc->type = BTPAIR_PASSIVE_MODE_WITH_SUCCESS;
    offset = BtPair_parsePassiveSuccessUrc(&urc->payload.passiveSuccess, ibuf, length);
  } else {
    urc->type = BTPAIR_NO_URC;  
  }

  if (0 == offset)
    urc->type = BTPAIR_NO_URC;

  return offset;
}

/****************************** END OF FILE **********************************/
