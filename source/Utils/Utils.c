/**
 * @file Utils.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Utils.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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
static double GSM_utilsAscii2Double(const char ibuf[], size_t length)
{
    if (0 == length)
        return 0.0;

    double result = 0.0;

    bool isNegative = ('-' == ibuf[0]);

    size_t i;
    for (i = isNegative ? 1 : 0; i < length && ibuf[i] != '.'; ++i) {
        result = 10 * result + (ibuf[i] - '0');
    }

    if (i == length)
        return result * (isNegative ? (-1) : 1);
    i++;

    double fract = 1.0;
    while (i < length) {
        fract *= 0.1;
        result += fract * (ibuf[i++] - '0');
    }

    return result * (isNegative ? (-1) : 1);
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
size_t GSM_UtilsGetString(const char input[], size_t ilen, char obuf[], size_t olen, char stok, char etok)
{
    const char *istart = input;
    const char *iend = input + ilen;

    memset(obuf, 0, olen);

    char *begin = strchr(istart, stok);
    if (!begin || (iend <= begin))
        return 0;

    begin += sizeof(stok);

    char *end = strchr(begin, etok);
    if (!end || (iend < end))
        return 0;

    size_t length = end - begin;
    if (olen < (length + 1))
        length = olen - 1;

    memcpy(obuf, begin, length);
    obuf[length] = '\0';

    return end - istart;
}

size_t GSM_UtilsGetInt(const char input[], size_t ilen, int *pi, char stok, char etok)
{
    const char *istart = input;
    const char *iend = input + ilen;

    *pi = 0;

    char *begin = strchr(istart, stok);
    if (!begin || (iend <= begin))
        return 0;

    begin += sizeof(stok);

    char *end = strchr(begin, etok);
    if (!end || (iend < end))
        return 0;    

    char num[10] = {0};
    size_t length = end - begin;
    if (sizeof(num) < (length + 1))
        length = sizeof(num) - 1;

    memcpy(num, begin, length);
    num[length] = '\0';

    *pi = atoi(num);

    return end - istart;
}

size_t GSM_UtilsGetDouble(const char input[], size_t ilen, double *pd, char stok, char etok)
{
    const char *istart = input;
    const char *iend = input + ilen;

    *pd = 0;

    char *begin = strchr(istart, stok);
    if (!begin || (iend <= begin))
        return 0;

    begin += sizeof(stok);

    char *end = strchr(begin, etok);
    if (!end || (iend < end))
        return 0;    

    char num[20] = {0};
    size_t length = end - begin;
    if (sizeof(num) < (length + 1))
        length = sizeof(num) - 1;

    memcpy(num, begin, length);
    num[length] = '\0';

    *pd = GSM_utilsAscii2Double(num, strlen(num));

    return end - istart;
}

size_t GSM_UtilsSkipReserved(const char input[], size_t ilen, char delim, size_t count)
{
    const char *istart = input;
    const char *iend = input + ilen;

    const char *end;
    size_t i;
    for (i = 0, end = input; end && (end < iend) && (i <= count); ++i) {
        end = strchr(end, delim);
        if (end) ++end;
    }

    size_t offset = 0;
    if ((istart < end) && (i == count+1))
        offset = end - 1 - istart;

    return offset;
}

/****************************** END OF FILE **********************************/
