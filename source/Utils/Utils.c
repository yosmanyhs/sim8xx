/**
 * @file Utils.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Utils.h"

#include <stdbool.h>
#include <stdlib.h>
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
static double GSM_utilsAscii2Double(const char ibuf[], size_t ilen)
{
  if (0 == ilen)
    return 0.0;

  double result = 0.0;

  bool isNegative = ('-' == ibuf[0]);

  size_t i;
  for (i = isNegative ? 1 : 0; i < ilen && ibuf[i] != '.'; ++i) {
    result = 10 * result + (ibuf[i] - '0');
  }

  if (i == ilen)
    return result * (isNegative ? (-1) : 1);
  i++;

  double fract = 1.0;
  while (i < ilen) {
    fract *= 0.1;
    result += fract * (ibuf[i++] - '0');
  }

  return result * (isNegative ? (-1) : 1);
}

static void GSM_UtilsReverse(char s[], size_t len)
{
  if (0 == len)
    return;

  size_t i, j;
  for (i = 0, j = len - 1; i < j; ++i, --j) {
    char c = s[i];
    s[i]   = s[j];
    s[j]   = c;
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
size_t
GSM_UtilsGetString(const char ibuf[], size_t ilen, char obuf[], size_t olen, char stok, char etok)
{
  const char *istart = ibuf;
  const char *iend   = ibuf + ilen;

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

size_t GSM_UtilsGetInt(const char ibuf[], size_t ilen, int32_t *pi, char stok, char etok)
{
  const char *istart = ibuf;
  const char *iend   = ibuf + ilen;

  *pi = 0;

  char *begin = strchr(istart, stok);
  if (!begin || (iend <= begin))
    return 0;

  begin += sizeof(stok);

  char *end = strchr(begin, etok);
  if (!end || (iend < end))
    return 0;

  char num[10]  = {0};
  size_t length = end - begin;
  if (sizeof(num) < (length + 1))
    length = sizeof(num) - 1;

  memcpy(num, begin, length);
  num[length] = '\0';

  *pi = atoi(num);

  return end - istart;
}

size_t GSM_UtilsGetDouble(const char ibuf[], size_t ilen, double *pd, char stok, char etok)
{
  const char *istart = ibuf;
  const char *iend   = ibuf + ilen;

  *pd = 0;

  char *begin = strchr(istart, stok);
  if (!begin || (iend <= begin))
    return 0;

  begin += sizeof(stok);

  char *end = strchr(begin, etok);
  if (!end || (iend < end))
    return 0;

  char num[20]  = {0};
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
  const char *iend   = input + ilen;

  const char *end;
  size_t i;
  for (i = 0, end = input; end && (end < iend) && (i <= count); ++i) {
    end = strchr(end, delim);
    if (end)
      ++end;
  }

  size_t offset = 0;
  if ((istart < end) && (i == count + 1))
    offset = end - 1 - istart;

  return offset;
}

size_t GSM_UtilsItoA(char obuf[], size_t olen, int32_t n)
{
  int32_t sign = n;

  if (n < 0)
    n = -n;

  size_t i = 0;
  do {
    obuf[i++] = n % 10 + '0';
    n /= 10;
  } while ((i < olen - 1) && (0 < n));

  if (0 == n) {
    if (sign < 0) {
      if (i < (olen - 2)) {
        obuf[i++] = '-';
      } else {
        memset(obuf, 0, olen);
        i = 0;
      }
    }

    obuf[i] = '\0';
    GSM_UtilsReverse(obuf, i);
  } else {
    memset(obuf, 0, olen);
    i = 0;
  }

  return i;
}

bool GSM_UtilsBeginsWith(const char *str, const char *pre)
{
  size_t slen = strlen(str);
  size_t plen = strlen(pre);

  if (slen < plen)
    return false;

  return 0 == strncasecmp(str, pre, plen);
}

bool GSM_UtilsEndsWith(const char *str, const char *post)
{
  size_t slen = strlen(str);
  size_t plen = strlen(post);

  if (slen < plen)
    return false;

  return 0 == strncasecmp(str + slen - plen, post, plen);
}

/****************************** END OF FILE **********************************/
