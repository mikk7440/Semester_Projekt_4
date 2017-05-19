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
#ifndef ACC_H_
#define ACC_H_

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
//extern INT16U v1;
//extern INT16U v2;
/*****************************   Functions   *******************************/

void acc_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data);
/*****************************************************************************
*   Input    : -
*   Output   : Button Event
*   Function : Test function
******************************************************************************/


/****************************** End Of Module *******************************/


#endif /* ACC_H_ */
