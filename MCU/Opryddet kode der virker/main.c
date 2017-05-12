/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: main.c
*
* PROJECT....: EMP
*
* DESCRIPTION: Assignment 1, main module. No main.h file.
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "swtimers.h"
#include "systick.h"
#include "button.h"
#include "button2.h"
#include "counter.h"
#include "countled.h"
#include "spi.h"

#include "gpio.h"

#include "events.h"

#include "uart.h"

#include "UI.h"
#include "regulering.h"


/*****************************    Defines    *******************************/
#define Idle              0
#define Forward           1
#define Backward          2
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
extern INT16S ticks;
INT32U RX_SPI;
INT32U TX_SPI;

INT8S c;

INT8U var;

INT8U pwm = 0;


//INT8U motor_1_speed;
//INT8U motor_2_speed;
//INT8U motor_1_enc;
//INT8U motor_2_enc;


/*****************************   Functions   *******************************/




int main(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{
    INT8U event1;
    INT8U alive_timer = TIM_500_MSEC;
    INT8U RX;



    disable_global_int();
    init_systick();
    init_gpio();
    init_spi2();
    init_uart0( 9600, 8, 1, 'n' ); // 1 stop bit no parity


    enable_global_int();


  // Loop forever.
    while(1)
    {
    // System part of the super loop.
    // ------------------------------
	while( !ticks );

	// The following will be executed every 5mS
    ticks--;

	if( ! --alive_timer )
	{
	  alive_timer        = TIM_500_MSEC;
	  GPIO_PORTD_DATA_R ^= 0x40; //D
	}

	// Application part of the super loop.
	// -----------------------------------

	event1 = select_button();

	sw1(event1);

	c = uartReadChar();

	UI_(c);

    }
  return( 0 );
}


/****************************** End Of Module *******************************/
