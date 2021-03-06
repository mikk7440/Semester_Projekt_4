/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: messages.h
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

#ifndef _MESSAGES_H
  #define _MESSAGES_H

/***************************** Include files *******************************/
#include <stdint.h>
#include "swtimers.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "timer0A.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
INT8U get_message_event(INT8U IDnumber);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

void put_message_event(INT8U IDnumber, INT8U event);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/
INT8U get_message_state(INT8U IDnumber);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

void put_message_state(INT8U IDnumber, INT8U state);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/

/****************************** End Of Module *******************************/
#endif

