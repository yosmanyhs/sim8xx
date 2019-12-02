/**
 * @file btconnect.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "btconnect.h"
#include "Utils.h"

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

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
bool BtConnectIsURC(const char *ibuf, size_t length)
{   
    const char *tag = "\r\n+BTCONNECTING";
    return (0 == strncasecmp(ibuf, tag, strlen(tag)));
}

size_t BtConnectParseURC(BtConnectingURC_t *urc, const char *ibuf, size_t length)
{
    size_t offset = 0;
    memset(urc, 0, sizeof(*urc));

    if (!BtConnectIsURC(ibuf, length))
        return 0;

    const char *next = ibuf;
    const char *end = ibuf + length;

    size_t n = GSM_UtilsGetString(next, end - next, urc->address, sizeof(urc->address), '"');
    if (n) {
        offset += n + 1;
        next += n + 1;
    } else {
        memset(urc, 0, sizeof(*urc));
        return 0;
    }
    
    n = GSM_UtilsGetString(next, end - next, urc->profile, sizeof(urc->profile), '"');
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

/****************************** END OF FILE **********************************/
