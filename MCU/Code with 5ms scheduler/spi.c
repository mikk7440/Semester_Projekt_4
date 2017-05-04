/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: emp.c
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
* 150228  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
/*****************************    Defines    *******************************/
#define SSI2_CR1_SSE    1
#define SSI2_CLK        4
#define SSI2_SS         5
#define SSI2_RX         6
#define SSI2_TX         7
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/


extern void init_spi2(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : Initializing SSI2 for FreeScale SPI
******************************************************************************/
{
    SYSCTL_RCGCSSI_R = SYSCTL_RCGCSSI_R2;   // Enable and provide a clock to SSI module 2 in Run mode.

//For legacy RCGC2 is used
//  SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R2; // Enable and provide a clock to GPIO Port B in Run mode.

    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;

    //GPIO 0xF0 = 0b11110000
    GPIO_PORTB_AFSEL_R = 0xF0;              // Enable Alternative Function for PB4-PB7(SSI2) in GPIO Alternative Function Select register.
    GPIO_PORTB_PCTL_R = (2<<16)|(2<<20)|(2<<24)|(2<<28);       // Enable SSI2 in GPIO Port Control
//  GPIO_PORTA_DR2R_R = 0xF0;               // By default, all GPIO pins have 2-mA drive.
    GPIO_PORTB_DEN_R = 0xF0;                // Digital enable for PB4-PB7
/*
     CPIO_PORTB_DEN_R &=
     CPIO_PORTB_DEN_R &=
     CPIO_PORTB_DEN_R &=
     CPIO_PORTB_DEN_R &=
*/

    GPIO_PORTB_DIR_R = 0xB0;        // PB4, PB5 and PB7 is outputs

    SSI2_CR1_R &= (0<<SSI2_CR1_SSE); // SSI control 1 - clear SSE(SSI Synchronous Serial Port Enable) bit - SSI operation is disabled
    SSI2_CR1_R = 0x00000000;
    SSI2_CC_R = 0x0;                // SSI Clock Configuration - system clock is chosen - based on clock and divisor
    SSI2_CPSR_R = 0xFE;             // SSI Clock Prescale - value form 2 to 254.
    SSI2_CR0_R = 0x8F;              // 00000000 1 0 00 1111 DSS=16bit FRF=Freescale SPI Frame Format SPH=1(0x80)(1<<7) SPO=0 SRC=0 (BR=SysClk/(CPSDVSR * (1 + SCR)) = x b/s(baud rate))
    // Kan vi argumentere for SPH?
    SSI2_CR1_R |= (1<<SSI2_CR1_SSE);// SSI control 1 - set SSE(SSI Synchronous Serial Port Enable) bit - SSI operation is enabled - MS=0(master)
}
/****************************** End Of Module *******************************/


