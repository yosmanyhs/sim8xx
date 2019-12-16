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
#include "Sim8xx_Config.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

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
} Buffer_t;

typedef struct Data_s {
  const char *data;
  size_t length;
} Data_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void GSM_BufferObjectInit(Buffer_t *this);

bool GSM_BufferPutChar(Buffer_t *this, char c);

Data_t GSM_BufferGetData(Buffer_t *this);

bool GSM_BufferClearData(Buffer_t *this, size_t length);

#endif /* BUFFER_H */

/****************************** END OF FILE **********************************/
