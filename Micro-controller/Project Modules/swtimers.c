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
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "messages.h"
#include "timer0A.h"


/*****************************    Defines    *******************************/

typedef struct timer_structure           // We make a variable time structure consisting of a timer counter and an timer event.
{
   INT16U timer_counter;                // Counter that holds the value of the counter
   INT8U  timer_event;                  // Timer event that holds the current event of the timer
} timer_structure;

#define MAX_TIMERS          8            // We definer the number of software timers we want

#define TIMER_NO_EVENT      0
#define TIME_OUT            1

/*****************************   Constants   *******************************/


/*****************************   Variables   *******************************/

timer_structure TIMER[MAX_TIMERS];  // Pool of timers

/*****************************   Functions   *******************************/

INT16U get_timer_counter(INT8U IDnumber)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    return TIMER[IDnumber].timer_counter;
}


void start_timer(INT8U IDnumber, INT8U event, INT16U timer_value)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    if(IDnumber < MAX_TIMERS)
    {
        TIMER[IDnumber].timer_counter = timer_value;
        TIMER[IDnumber].timer_event = event;
    }
}

void check_timers(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    for(int i = 0; i < MAX_TIMERS; i++)
    {
        if(!(TIMER[i].timer_counter == 0))
        {
            TIMER[i].timer_counter--;
            if(TIMER[i].timer_counter == 0)
            {
                put_message_event(TIMER[i].timer_event, TIME_OUT);
            }
        }
    }
}

void init_timers()                                            // Clearing all the timers
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    for (int i = 0; i < MAX_TIMERS; i++)
    {
        TIMER[i].timer_counter = 0;
        TIMER[i].timer_event = TIMER_NO_EVENT;
    }
}

/****************************** End Of Module *******************************/








