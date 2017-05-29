/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: swtimers.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 080219  MoH    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "emp_type.h"
#include "global_def.h"
/*****************************    Defines    *******************************/
#define MAX_MESSAGES    8

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8U   message_event[MAX_MESSAGES];
INT8U   message_state[MAX_MESSAGES];

/*****************************   Functions   *******************************/

INT8U get_message_event(INT8U IDnumber)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    INT8U event = 0;
    if(IDnumber < MAX_MESSAGES)
    {
        if(!(message_event[IDnumber] == 0))        // If there is an event messages, then
        {
            event = message_event[IDnumber];
            message_event[IDnumber] = 0;
        }
    }
    return event;
}


void put_message_event(INT8U IDnumber, INT8U event)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    if(message_event[IDnumber] < MAX_MESSAGES)
    {
        message_event[IDnumber] = event;
    }
}


INT8U get_message_state(INT8U IDnumber)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    INT8U state = 0;
    if(IDnumber < MAX_MESSAGES)
    {
        if(!(message_state[IDnumber] == 0))        // If there is an event messages, then
        {
            state = message_state[IDnumber];
            message_state[IDnumber] = 0;
        }
    }
    return state;
}


void put_message_state(INT8U IDnumber, INT8U state)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    if(message_state[IDnumber] < MAX_MESSAGES)
    {
        message_state[IDnumber] = state;
    }
}
/****************************** End Of Module *******************************/

