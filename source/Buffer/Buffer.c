/**
 * @file Buffer.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Buffer.h"
#include "Interface/Os.h"

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
void GSM_BufferObjectInit(GSM_Buffer_t *this)
{
  OS_LockBuffer();
  memset(this, 0, sizeof(*this));
  OS_UnlockBuffer();
}

bool GSM_BufferPushChar(GSM_Buffer_t *this, char c)
{
  OS_LockBuffer();

  if (this->rdindex == this->wrindex) {
    this->rdindex = 0;
    this->wrindex = 0;
    memset(this->buffer, 0, sizeof(this->buffer));
  }

  bool result = false;
  if (this->wrindex < SIM8XX_INPUT_BUFFER_LENGTH) {
    this->buffer[this->wrindex] = c;
    ++this->wrindex;
    result = true;
  }

  OS_UnlockBuffer();

  return result;
}

GSM_BufferData_t GSM_BufferGetData(GSM_Buffer_t *this)
{
  GSM_BufferData_t data = {0};

  OS_LockBuffer();
  if (this->rdindex <= this->wrindex) {
    data.data   = &this->buffer[this->rdindex];
    data.length = this->wrindex - this->rdindex;
  }
  OS_UnlockBuffer();

  return data;
}

bool GSM_BufferPopData(GSM_Buffer_t *this, size_t length)
{
  OS_LockBuffer();

  bool result = false;
  if ((this->rdindex + length) <= this->wrindex) {
    this->rdindex += length;
    result = true;
  }

  OS_UnlockBuffer();

  return result;
}

size_t GSM_BufferGetLength(GSM_Buffer_t *this)
{
  size_t n = 0;

  OS_LockBuffer();

  if (this->rdindex <= this->wrindex)
    n = this->wrindex - this->rdindex;

  OS_UnlockBuffer();

  return n;
}

/****************************** END OF FILE **********************************/
