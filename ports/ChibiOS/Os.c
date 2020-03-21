/**
 * @file Os.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Interface/Os.h"
#include "ch.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define GUARD_TIME_IN_MSEC 100

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static mutex_t modemLock;
static mutex_t parserLock;
static mutex_t bufferLock;
static semaphore_t modemReady;
static semaphore_t guardSync;
static virtual_timer_t guardTimer;
static thread_reference_t writer;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
void OS_guardTimerCallback(void *p)
{
  (void)p;
  chSysLockFromISR();
  chSemResetI(&guardSync, 1);
  chVTSetI(&guardTimer, TIME_MS2I(GUARD_TIME_IN_MSEC), OS_guardTimerCallback, NULL);
  chSysUnlockFromISR();
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void OS_Init(void)
{
  chMtxObjectInit(&modemLock);
  chMtxObjectInit(&parserLock);
  chMtxObjectInit(&bufferLock);
  chSemObjectInit(&modemReady, 1);
  chSemObjectInit(&guardSync, 0);
  chVTObjectInit(&guardTimer);
  writer = NULL;
}

void OS_LockModem(void)
{
  chMtxLock(&modemLock);
}

void OS_UnlockModem(void)
{
  chMtxUnlock(&modemLock);
}

void OS_WaitIfModemIsNotReady(void)
{
  while(MSG_OK != chSemWait(&modemReady))
    ;
  chSemSignal(&modemReady);
}

void OS_ModemIsReady(void)
{
  chSemReset(&modemReady, 1);
}

void OS_ModemIsNotReady(void)
{
  chSysLock();
  chSemResetI(&modemReady, 0);
  chVTResetI(&guardTimer);
  chSemResetI(&guardSync, 0);
  chSysUnlock();
}

void OS_LockParser(void)
{
  chMtxLock(&parserLock);
}

void OS_UnlockParser(void)
{
  chMtxUnlock(&parserLock);
}

void OS_LockBuffer(void)
{
  chMtxLock(&bufferLock);
}

void OS_UnlockBuffer(void)
{
  chMtxUnlock(&bufferLock);
}

void OS_WaitGuardTimeToPass(void)
{
  while (MSG_OK != chSemWait(&guardSync))
    ;
}

void OS_StartGuardTimer(void)
{
  chSysLock();
  chSemResetI(&guardSync, 0);
  chVTSetI(&guardTimer, TIME_MS2I(GUARD_TIME_IN_MSEC), OS_guardTimerCallback, NULL);
  chSysUnlock();
}

void OS_SleepMilliSeconds(uint32_t msecs)
{
  chThdSleepMilliseconds(msecs);
}

OS_Error_t OS_WaitForResponseWithTimeout(uint32_t timeoutInMsec)
{
  chSysLock();
  msg_t msg = chThdSuspendTimeoutS(&writer, chTimeMS2I(timeoutInMsec));
  writer    = NULL;
  chSysUnlock();

  return (MSG_TIMEOUT == msg) ? OS_TIMEOUT : OS_NO_ERROR;
}

void OS_WakeUpThreadWaitingForResponse(void)
{
  if (writer)
    chThdResume(&writer, MSG_OK);
}

/****************************** END OF FILE **********************************/
