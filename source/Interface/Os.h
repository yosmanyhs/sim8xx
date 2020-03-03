/**
 * @file Os.h
 * @brief
 */

#ifndef OS_H
#define OS_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stdint.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef enum {
    OS_NO_ERROR,
    OS_TIMEOUT,
} OS_Error_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void OS_Init(void);

void OS_LockModem(void);

void OS_UnlockModem(void);

void OS_WaitIfModemIsNotReady(void);

void OS_ModemIsReady(void);

void OS_ModemIsNotReady(void);

void OS_LockParser(void);

void OS_UnlockParser(void);

void OS_LockBuffer(void);

void OS_UnlockBuffer(void);

void OS_WaitGuardTimeToPass(void);

void OS_StartGuardTimer(void);

void OS_SleepMilliSeconds(uint32_t msecs);

OS_Error_t OS_WaitForResponseWithTimeout(uint32_t timeoutInMs);

void OS_WakeUpThreadWaitingForResponse(void);

#endif /* OS_H */

/****************************** END OF FILE **********************************/
