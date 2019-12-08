/**
 * @file Btdisconn.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "btdisconn.h"
#include "Utils.h"

#include <string.h>
#include <stdio.h>

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
bool BtDisconnIsBtDisconn(const char *ibuf, size_t length)
{
    const char *tag = "\r\n+BTDISCONN:";    
    return (0 == strncasecmp(ibuf, tag, strlen(tag)));
}

#if !defined(TEST)
static
#endif
size_t BtDisconn_parseBtDisconn(BtDisconn_DisconnectedURC_t *urc, const char *ibuf, size_t length)
{
    size_t offset = 0;
    memset(urc, 0, sizeof(*urc));

    if (!BtDisconnIsBtDisconn(ibuf, length))
        return 0;

    const char *next = ibuf;
    const char *end = ibuf + length;

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
bool BtDisconnIsURC(const char *ibuf, size_t length)
{
    return BtDisconnIsBtDisconn(ibuf, length);
}

size_t BtDisconnParseURC(BtDisconnURC_t *urc, const char *ibuf, size_t length)
{
    size_t offset = 0;

    if (BtDisconnIsBtDisconn(ibuf, length)) {
        urc->type = BTDISCONN_DISCONNECTED;
        offset = BtDisconn_parseBtDisconn(&urc->payload.disconn, ibuf, length);
    } else {
        urc->type = BTDISCONN_NO_URC;
    }

    if (0 == offset)
        urc->type = BTDISCONN_NO_URC;

    return offset;
}

/****************************** END OF FILE **********************************/
