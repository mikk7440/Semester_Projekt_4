/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: gpio.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
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
#include "gpio.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/


void init_gpio()
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{

  // Enable the GPIO port that is used for the on-board LED.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF | SYSCTL_RCGC2_GPIOD | SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOA | SYSCTL_RCGC2_GPIOB;

  // Insert a cycle after enabling the peripheral.
  __asm(" nop");

  // Set the direction as output/input
  GPIO_PORTC_DIR_R = 0b11110000;
  GPIO_PORTD_DIR_R = 0b01001100;
  GPIO_PORTF_DIR_R = 0b00001110;

  // Enable the GPIO pins for digital function
  GPIO_PORTC_DEN_R = 0b11110000;
  GPIO_PORTD_DEN_R = 0b01001100;
  GPIO_PORTF_DEN_R = 0b00011111;

  // Enable internal pull-up
  GPIO_PORTF_PUR_R = 0b00010001;
}

/****************************** End Of Module *******************************/
