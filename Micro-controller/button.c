/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: button.c
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
#include "swtimers.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
#include "global_def.h"
#include "messages.h"
#include "timer0A.h"

/*****************************    Defines    *******************************/
// Button states:
#define IDLE                0
#define FIRST_PUSH          1
#define BUTTON_RELEASE      2
#define DOUBLE_PRESS        3
#define LONG_PUSH           4

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8U button_state = IDLE;
INT8U button_event = BE_NO_EVENT;

/*****************************   Functions   *******************************/
INT8U button_pushed()
{
    return !(GPIO_PORTF_DATA_R & 0b10000); // Is button SW1 at PORTF4 turned on
}


void button()
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    INT8U time_out;
    time_out = get_message_event(EVENT_TO_BUTTON);

    switch ( button_state )
    {
    case IDLE:
        if(button_pushed())                     // If button is pushed
        {
            button_state = FIRST_PUSH;
            start_timer(ID_TIMER_BUTTON, EVENT_TO_BUTTON, MS(2000));
        }
    break;

    case FIRST_PUSH:
        if( time_out )                          // If time out
        {
            button_state = LONG_PUSH;
            put_message_event(EVENT_TO_BUTTON, BE_LONG_PUSH);
        }else{
         if(!button_pushed())                   // Else if the button is released
         {
            button_state = BUTTON_RELEASE;
            start_timer(ID_TIMER_BUTTON, EVENT_TO_BUTTON, MS(100));
         }
        }
    break;

    case BUTTON_RELEASE:
        if( time_out )
        {
            button_state = IDLE;
            put_message_event(EVENT_TO_BUTTON, BE_SINGLE_PUSH);
        }else{
            if(button_pushed())
            {
                button_state = DOUBLE_PRESS;
            }
        }
    break;

    case DOUBLE_PRESS:
        if(!button_pushed())
        {
            put_message_event(EVENT_TO_BUTTON, BE_DOUBLE_PUSH);
            button_state = IDLE;
        }

    break;

    case LONG_PUSH:
        if( !button_pushed() )
        {
            button_state = IDLE;
        }

    break;
    }

}

/****************************** End Of Module *******************************/








