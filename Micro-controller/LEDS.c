/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: LEDS.c
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
#include "LEDS.h"
#include "emp_type.h"
#include "global_def.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/
void select_color(INT16U color)
{
    switch(color)
        {
        case NO_LIGTH:
            GPIO_PORTF_DATA_R &= !(0b1110);
            break;

        case GREEN:
            GPIO_PORTF_DATA_R &= !(0b1110);
            GPIO_PORTF_DATA_R |= 0b1000;
            break;

        case BLUE:
            GPIO_PORTF_DATA_R &= !(0b1110);
            GPIO_PORTF_DATA_R |= 0b0100;
            break;

        case CYAN:
            GPIO_PORTF_DATA_R &= !(0b1110);
            GPIO_PORTF_DATA_R |= 0b1100;
            break;

        case RED:
            GPIO_PORTF_DATA_R &= !(0b1110);
            GPIO_PORTF_DATA_R |= 0b0010;
            break;

        case YELLOW:
            GPIO_PORTF_DATA_R &= !(0b1110);
            GPIO_PORTF_DATA_R |= 0b1010;
            break;

        case MAGENTA:
            GPIO_PORTF_DATA_R &= !(0b1110);
            GPIO_PORTF_DATA_R |= 0b0110;
            break;

        case WHITE:
            GPIO_PORTF_DATA_R |= (0b1110);
            break;

        default:
        break;
        };
}



/****************************** End Of Module *******************************/
