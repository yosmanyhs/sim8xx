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
static thread_reference_t writer;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void OS_Init(void)
{
  chMtxObjectInit(&modemLock);
  chMtxObjectInit(&parserLock);
  chMtxObjectInit(&bufferLock);
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

OS_Error_t OS_WaitForResponseWithTimeout(uint32_t timeoutInMsec)
{
  chSysLock();
  msg_t msg    = chThdSuspendTimeoutS(&writer, chTimeMS2I(timeoutInMsec));
  writer = NULL;
  chSysUnlock();

  return (MSG_TIMEOUT == msg) ? OS_TIMEOUT : OS_NO_ERROR;
}

void OS_WakeUpThreadWaitingForResponse(void)
{
  if (writer)
    chThdResume(&writer, MSG_OK);
}

/****************************** END OF FILE **********************************/
