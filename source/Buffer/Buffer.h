/**
 * @file Buffer.h
 * @brief
 */

#ifndef BUFFER_H
#define BUFFER_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stddef.h>
#include <stdbool.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define SIM8XX_INPUT_BUFFER_LENGTH       256

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef struct Buffer_s {
  char buffer[SIM8XX_INPUT_BUFFER_LENGTH];
  size_t wrindex;
  size_t rdindex;
} GSM_Buffer_t;

typedef struct Data_s {
  const char *data;
  size_t length;
} GSM_BufferData_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void GSM_BufferObjectInit(GSM_Buffer_t *this);

bool GSM_BufferPutChar(GSM_Buffer_t *this, char c);

GSM_BufferData_t GSM_BufferGetData(GSM_Buffer_t *this);

bool GSM_BufferClearData(GSM_Buffer_t *this, size_t length);

#endif /* BUFFER_H */

/****************************** END OF FILE **********************************/
