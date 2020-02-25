/**
 * @file Utils.h
 * @brief
 */

#ifndef UTILS_H
#define UTILS_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
size_t GSM_UtilsGetString(const char ibuf[], size_t ilen, char obuf[], size_t olen, char stok, char etok);

size_t GSM_UtilsGetInt(const char ibuf[], size_t ilen, int32_t *pi, char stok, char etok);

size_t GSM_UtilsGetDouble(const char ibuf[], size_t ilen, double *pd, char stok, char etok);

size_t GSM_UtilsSkipReserved(const char ibuf[], size_t ilen, char delim, size_t count);

size_t GSM_UtilsItoA(char obuf[], size_t olen, int32_t n);

bool GSM_UtilsBeginsWith(const char *str, const char *pre);

bool GSM_UtilsEndsWith(const char *str, const char *post);

#endif /* UTILS_H */

/****************************** END OF FILE **********************************/
