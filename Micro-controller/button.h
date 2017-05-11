/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: button.h
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
* 090215  MoH   Module created.
*
*****************************************************************************/

#ifndef BUTTON_H_
#define BUTTON_H_

/***************************** Include files *******************************/
#include <stdint.h>
#include "swtimers.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
#include "global_def.h"
#include "timer0A.h"


/*****************************    Defines    *******************************/


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
INT8U button_pushed();
void button();

/*****************************************************************************
*   Input    : -
*   Output   : Button Event
*   Function : Test function
******************************************************************************/


/****************************** End Of Module *******************************/
#endif /*BUTTON_H_*/


