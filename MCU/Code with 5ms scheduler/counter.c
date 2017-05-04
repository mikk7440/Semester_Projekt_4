/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: counter.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "events.h"
#include "swtimers.h"
/*****************************    Defines    *******************************/
// Counter States
// --------------
#define CS_MAN_UP    0
#define CS_MAN_DOWN  1
#define CS_AUTO_UP   2
#define CS_AUTO_DOWN 3

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/


INT8U counter( INT8U event )
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  static INT8U  counter_state =  CS_MAN_UP;
  static INT8U  counter_value = 0;
  static INT16U counter_timer = 0;

  if( counter_timer )
    if( ! --counter_timer )
      event = TE_TIMEOUT;
  
  if( event )
  {
    switch( counter_state )
    {
	    case CS_MAN_UP:
	      switch( event )
	      {
	        case BE_SINGLE_PUSH:
	          counter_value++;
	          break;
	        case BE_DOUBBLE_PUSH:
		        counter_state = CS_MAN_DOWN;
	          break;
	        case BE_LONG_PUSH:
	          counter_state = CS_AUTO_UP;
		        counter_timer = TIM_200_MSEC;
	          break;
	        default:
	          break;
	      }
	      break;
	    case CS_MAN_DOWN:
	      switch( event )
	      {
	        case BE_SINGLE_PUSH:
	          counter_value--;
	          break;
	        case BE_DOUBBLE_PUSH:
		        counter_state = CS_MAN_UP;
	          break;
	        case BE_LONG_PUSH:
		        counter_timer = TIM_200_MSEC;
		        counter_state = CS_AUTO_DOWN;
	          break;
	        default:
	          break;
	      }
	      break;
	    case CS_AUTO_UP:
	      switch( event )
	      {
	        case TE_TIMEOUT:
	          counter_value++;
	          counter_timer = TIM_200_MSEC;
	          break;
	        default:
	          counter_state = CS_MAN_UP;
	          break;
	      }
	      break;
	    case CS_AUTO_DOWN:
	      switch( event )
	      {
	        case TE_TIMEOUT:
	          counter_value--;
	          counter_timer = TIM_200_MSEC;
	          break;
	        default:
	          counter_state = CS_MAN_UP;
	          break;
	      }
	      break;
	    default:
	      break;
    }
  }
  return( counter_value );
}


/****************************** End Of Module *******************************/












