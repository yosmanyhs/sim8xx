/**
 * @file At.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "At.h"

#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define CRLF       "\r\n"
#define CRLFLENGTH 2
/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static const char *StatusStrings[] = {
    [AT_CMD_OK]          = "OK",
    [AT_CMD_CONNECT]     = "CONNECT",     
    [AT_CMD_RING]        = "RING",  
    [AT_CMD_NO_CARRIER]  = "NO CARRIER",        
    [AT_CMD_ERROR]       = "ERROR",   
    [AT_CMD_NO_DIALTONE] = "NO DIALTONE",         
    [AT_CMD_BUSY]        = "BUSY",  
    [AT_CMD_NO_ANSWER]   = "NO ANSWER",       
    [AT_CMD_PROCEEDING]  = "PROCEEDING",     
    [AT_CMD_WAIT_FOR_USER_DATA] = "> ",
};

static const size_t StatusStringLength[] = {
    [AT_CMD_INVALID]     = 0,
    [AT_CMD_OK]          = 2, 
    [AT_CMD_CONNECT]     = 7,
    [AT_CMD_RING]        = 4, 
    [AT_CMD_NO_CARRIER]  = 10,     
    [AT_CMD_ERROR]       = 5,
    [AT_CMD_NO_DIALTONE] = 11,      
    [AT_CMD_BUSY]        = 4,
    [AT_CMD_NO_ANSWER]   = 9,   
    [AT_CMD_PROCEEDING]  = 10,    
    [AT_CMD_WAIT_FOR_USER_DATA] = 2,
};

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
 static bool AT_isStatus(AT_CommandStatus_t st, const char *ibuf, size_t length)
 {
    if ((2 == length) && (AT_CMD_WAIT_FOR_USER_DATA == st)) {
        return 0 == strncasecmp(ibuf, StatusStrings[st], StatusStringLength[st]);
    }
    
    if (length < (StatusStringLength[st] + 2 * CRLFLENGTH))
        return false;  
    
    const char *str = ibuf;
    if (0 == strncasecmp(str, CRLF, CRLFLENGTH)) {
        length -= CRLFLENGTH;
        str += CRLFLENGTH;
    } else { 
        return false;
    }
    
    if (0 == strncasecmp(ibuf + 2, StatusStrings[st], StatusStringLength[st])) {
        length -= StatusStringLength[st];
        str += StatusStringLength[st];
    } else {
        return false;
    }

    return 0 == strncasecmp(str, CRLF, CRLFLENGTH);
 }

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
size_t AT_CommandStatusParse(const char *ibuf, size_t length, AT_CommandStatus_t *status)
{
    *status = AT_CMD_INVALID;
    size_t n = 0;

    if (AT_isStatus(AT_CMD_OK, ibuf, length)) {
        *status = AT_CMD_OK;
        n = StatusStringLength[AT_CMD_OK] + 2 * CRLFLENGTH;
    } 
    else if (AT_isStatus(AT_CMD_WAIT_FOR_USER_DATA, ibuf, length)) {
        *status = AT_CMD_WAIT_FOR_USER_DATA;
        n = StatusStringLength[AT_CMD_WAIT_FOR_USER_DATA];
    } 
    else if (AT_isStatus(AT_CMD_CONNECT, ibuf, length)) {
        *status = AT_CMD_CONNECT;
        n = StatusStringLength[AT_CMD_CONNECT] + 2 * CRLFLENGTH;
    } 
    else if (AT_isStatus(AT_CMD_RING, ibuf, length)) {
        *status = AT_CMD_RING;
        n = StatusStringLength[AT_CMD_RING] + 2 * CRLFLENGTH;
    } 
    else if (AT_isStatus(AT_CMD_NO_CARRIER, ibuf, length)) {
        *status = AT_CMD_NO_CARRIER;
        n = StatusStringLength[AT_CMD_NO_CARRIER] + 2 * CRLFLENGTH;
    } 
    else if (AT_isStatus(AT_CMD_ERROR, ibuf, length)) {
        *status = AT_CMD_ERROR;
        n = StatusStringLength[AT_CMD_ERROR] + 2 * CRLFLENGTH;
    } 
    else if (AT_isStatus(AT_CMD_NO_DIALTONE, ibuf, length)) {
        *status = AT_CMD_NO_DIALTONE;
        n = StatusStringLength[AT_CMD_NO_DIALTONE] + 2 * CRLFLENGTH;
    } 
    else if (AT_isStatus(AT_CMD_BUSY, ibuf, length)) {
        *status = AT_CMD_BUSY;
        n = StatusStringLength[AT_CMD_BUSY] + 2 * CRLFLENGTH;
    } 
    else if (AT_isStatus(AT_CMD_NO_ANSWER, ibuf, length)) {
        *status = AT_CMD_NO_ANSWER;
        n = StatusStringLength[AT_CMD_NO_ANSWER] + 2 * CRLFLENGTH;
    } 
    else if (AT_isStatus(AT_CMD_PROCEEDING, ibuf, length)) {
        *status = AT_CMD_PROCEEDING;
        n = StatusStringLength[AT_CMD_PROCEEDING] + 2 * CRLFLENGTH;
    } 
    else if (AT_isStatus(AT_CMD_CONNECT, ibuf, length)) {
        *status = AT_CMD_CONNECT;
        n = StatusStringLength[AT_CMD_CONNECT] + 2 * CRLFLENGTH;
    } 
    else {
        *status = AT_CMD_INVALID;
        n = StatusStringLength[AT_CMD_INVALID];  
    }

    return n;
}

/****************************** END OF FILE **********************************/
