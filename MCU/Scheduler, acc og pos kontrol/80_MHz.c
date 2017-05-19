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
#include "timer0A.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/


void init_80_MHz(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    //For 80 Mhz, SYSDIV2 = 0x2, SYSDIV2LSB = 0.
    SYSCTL_RCC_R |= SYSCTL_RCC_USESYSDIV; // Enable System Clock Divider

    //RCC2 overrides the RCC equivalent register fields
    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;        // Use RCC2
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;         // Divide PLL as 400 MHz vs. 200
    SYSCTL_RCC2_R &= ~(SYSCTL_RCC2_SYSDIV2LSB);  // Additional LSB for SYSDIV2 - SYSDIV2LSB = 0.
    SYSCTL_RCC2_R &= ~(SYSCTL_RCC2_PWRDN2); //      // set PPL to normal mode
    SYSCTL_RCC2_R &= ~(SYSCTL_RCC2_BYPASS2);        // PLL Bypass 2
    SYSCTL_RCC2_R &= ~(SYSCTL_RCC2_SYSDIV2_M);      // clear sysdiv2
    SYSCTL_RCC2_R |= (2 << SYSCTL_RCC2_SYSDIV2_S); // SYSCTL_RCC2_SYSDIV2_S = 23 - SYSDIV2 = 0x2
}

/****************************** End Of Module *******************************/








