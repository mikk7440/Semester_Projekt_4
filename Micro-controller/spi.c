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
#include "global_def.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "timer0A.h"
/*****************************    Defines    *******************************/
#define SSI2_CR1_SSE    1
#define SSI2_CLK        4
#define SSI2_SS         5
#define SSI2_RX         6
#define SSI2_TX         7
/*****************************   Constants   *******************************/
INT16U spi_data;
/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/


extern void init_spi2()
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R2;   // Enable and provide a clock to SSI module 2 in Run mode.
   // SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R2; // Enable and provide a clock to GPIO Port B in Run mode.
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;     // Is done in main

    //GPIO 0xF0 = 0b11110000
    GPIO_PORTB_AFSEL_R |= 0xF0;              // Enable Alternative Function for PB4-PB7(SSI2) in GPIO Alternative Function Select register.
    GPIO_PORTB_PCTL_R |= (2<<16)|(2<<20)|(2<<24)|(2<<28);       // Enable SSI2 in GPIO Port Control
    GPIO_PORTB_DEN_R |= 0xF0;                // Digital enable for PB4-PB7

    GPIO_PORTB_DIR_R |= 0xB0;        // PB4, PB5 and PB7 is outputs

    SSI2_CR1_R &= (0<1);            // SSI control 1 - clear SSE(SSI Synchronous Serial Port Enable) bit - SSI operation is disabled
    SSI2_CR1_R = 0x00000000;
    SSI2_CC_R = 0x0;                // SSI Clock Configuration - system clock is chosen - based on clock and divisor
    SSI2_CPSR_R = 0xFE;             // SSI Clock Prescale - value form 2 to 254.
    SSI2_CR0_R = 0x8F;              // 00000000 1 0 00 1111 DSS=16bit FRF=Freescale SPI Frame Format SPH=1(0x80)(1<<7) SPO=0 SRC=0 (BR=SysClk/(CPSDVSR * (1 + SCR)) = x b/s(baud rate))
    // Kan vi argumentere for SPH?
    SSI2_CR1_R |= (1<<1);           // SSI control 1 - set SSE(SSI Synchronous Serial Port Enable) bit - SSI operation is enabled - MS=0(master)
}

extern void init_spi0()
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;   // Enable and provide a clock to SSI module 0 in Run mode.
    //SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R0; // Enable and provide a clock to GPIO Port A in Run mode.
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;


    GPIO_PORTA_AFSEL_R |= (1<<2)|(1<<3)|(1<<4)|(1<<5) ;              // Enable Alternative Function for PA2-PA5 (SSI0) in GPIO Alternative Function Select register.
    GPIO_PORTA_PCTL_R |= (2<<8)|(2<<12)|(2<<16)|(2<<20);                   // Enable SSI0 in GPIO Port Control
    GPIO_PORTA_DEN_R |= (1<<2)|(1<<3)|(1<<4)|(1<<5);                  // Digital enable for PA2-PA5

    GPIO_PORTA_DIR_R |= 0b00101100;        // PA2 (CLK), PA3 (CS) and PA5 (TX) is outputs
    GPIO_PORTA_DIR_R &= 0b11101111;        // PA2 (CLK), PA3 (CS) and PA5 (TX) is outputs

    SSI0_CR1_R &= (0<<1);            // SSI control 1 - clear SSE(SSI Synchronous Serial Port Enable) bit - SSI operation is disabled
    SSI0_CR1_R = 0x00000000;         // Setup for master mode
    SSI0_CC_R = 0x0;                // SSI Clock Configuration - system clock is chosen - based on clock and divisor
    SSI0_CPSR_R = 0b1010;             // SSI Clock Prescale - value form 2 to 254.
    SSI0_CR0_R = 0b11001111;              // 00000000 1 0 00 1111 DSS=16bit FRF=Freescale SPI Frame Format SPH=1, SPO=1 SRC=0 (BR=SysClk/(CPSDVSR * (1 + SCR)) = x b/s(baud rate))
    SSI0_CR1_R |= (1<<1);           // SSI control 1 - set SSE(SSI Synchronous Serial Port Enable) bit - SSI operation is enabled - MS=0(master)


     INT16U data_mode;
    data_mode = 0b0011000100001011;
    SSI0_DR_R = data_mode;
    while( (SSI0_SR_R & (1<<0)) == 0 );

    INT16U measu_mode;
    measu_mode = 0b0010110100001000;
    SSI0_DR_R = measu_mode;
    while( (SSI0_SR_R & (1<<0)) == 0 );

}

BOOLEAN spi_ready_tx()
{
    if((SSI0_SR_R & (1<<0)) )         // Hvis vi kigger p� SSI_0 og transmit bufferen er tom
        return TRUE;
    return FALSE;
}

BOOLEAN spi_ready_rx()
{
    if((SSI0_SR_R & (1<<2)))         // Hvis vi kigger p� SSI_0 og recive bufferen er full
        return TRUE;
    return FALSE;
}

INT16U spi_get_data()
{
    return SSI0_DR_R;
}

extern void spi_tx_task(INT8U task_id, INT8U task_state, INT8U event, INT8U data)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    if(get_queue(QUEUE_SPI_TX_0, &spi_data, WAIT_FOREVER))
    {
        SSI0_DR_R = spi_data;
    }
}


extern void spi_rx_task(INT8U task_id, INT8U task_state, INT8U event, INT8U data)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    if(spi_ready_rx())
    {
        put_queue( QUEUE_SPI_RX_0, spi_get_data(), WAIT_FOREVER);
    }

}
/****************************** End Of Module *******************************/

