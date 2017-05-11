/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: swtimers.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef _SWTIMERS_H
  #define _SWTIMERS_H

/***************************** Include files *******************************/

#include <stdint.h>
#include "emp_type.h"
#include "messages.h"
#include "timer0A.h"


/*****************************    Defines    *******************************/
#define TICK_TIME   5       // tick time in milli seconds.
#define MS(time_in_msec)  time_in_msec / TICK_TIME


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
INT16U get_timer_counter(INT8U IDnumber);
/*****************************************************************************
*   Input    : The timer ID number
*   Output   : The timer value
*   Function : -
******************************************************************************/

void start_timer(INT8U IDnumber, INT8U event, INT16U timer_value);
/*****************************************************************************
*   Input    : Timer ID number, what event we want to trigger, and the timer value
*   Output   : -
*   Function : Starts the timer
******************************************************************************/

void check_timers(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

void init_timers(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Resets all timers.
******************************************************************************/

/****************************** End Of Module *******************************/
#endif
