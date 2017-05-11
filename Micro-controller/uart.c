/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: uart.c
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
#include "rtcs.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

 INT16U ch;
/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/
void init_uart()
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    SYSCTL_RCGCUART_R |= 0b1;       // Enable UART 0. - Enable the UART module using the RCGCUART register (see page 343).
    SYSCTL_RCGCGPIO_R |= 0b1;       // Enable Port A - RX = PA0, TX = PA1 - Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 339). To find out which GPIO port to enable, refer to Table 23-5 on page 1346.
    GPIO_PORTA_AFSEL_R |= 0b11;     // Set the GPIO AFSEL bits for the appropriate pins (see page 669). To determine which GPIOs to configure, see Table 23-4 on page 1339.
    GPIO_PORTA_PCTL_R |= 0b10001;   // Enable PA0 and PA1. -  Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate pins (see page 686 and Table 23-5 on page 1346).
    GPIO_PORTA_DEN_R |= 0b11;       // Enable port PA0, PA1 for digital pins.

    // Finding the baud-rate divisor:
    // BRD = 16,000,000 / (16 * 9600) = 104.166666667
    // To get the fraction divisor:
    // UARTFBRD[DIVFRAC] = integer(0.166666667 * 64 + 0.5) = 11

    UART0_CTL_R &= ~(0b0);          // Disable the UART by clearing the UARTEN bit in the UARTCTL register.
    UART0_IBRD_R = 104;             // Write the integer portion of the BRD to the UARTIBRD register.
    UART0_FBRD_R = 11;              // Write the fractional portion of the BRD to the UARTFBRD register..
    UART0_LCRH_R = 0b1100000;       // 8-bits, no parity, 1 stop bit. - Write the desired serial parameters to the UARTLCRH register (in this case, a value of 0x0000.0060).
    UART0_CC_R = 0b0;               // Configure the UART clock source by writing to the UARTCC register.
    /*
    //Interrupt
    UART0_ICR_R |= 0b110000;
    UART0_IM_R |= 0b110000;
    NVIC_EN0_R |= 0b100000;
    */

    UART0_CTL_R |= 0b1100000001;    // Enable UART0, TX and RX - Enable the UART by setting the UARTEN bit in the UARTCTL register.
}

BOOLEAN uart_ready_tx()
{
    return (UART_FR_TXFE & UART0_FR_R);  // Returns true is the transmitter is full
}

BOOLEAN uart_ready_rx()
{
    return (UART_FR_RXFF & UART0_FR_R);  // Returns true is the reciver is full
}

BOOLEAN uart_put_que(INT16U ch){
  return put_queue(QUEUE_UART_TX, ch, WAIT_FOREVER);
}

BOOLEAN uart_get_que(INT16U *pch){
  return get_queue(QUEUE_UART_RX, pch, WAIT_FOREVER);
}


INT16U uart_get_char()
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    return(UART0_DR_R);
}

extern void uart_tx_task(INT8U task_id, INT8U task_state, INT8U event, INT16U data)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    char ch;
    if(get_queue( QUEUE_UART_TX, &ch, WAIT_FOREVER ))
        UART0_DR_R = ch;
}

extern void uart_rx_task(INT8U task_id, INT8U task_state, INT8U event, INT16U data)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    if(uart_ready_rx())
        put_queue( QUEUE_UART_RX, uart_get_char(), WAIT_FOREVER);

}

void UART0_Handler(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    if (UART0_FR_R & (1<<7))               // If the UART transmission FIFO is empty
    {
       UART0_ICR_R |= (1<<5);              // Reset the TX interrupt
    }

    if(UART0_FR_R & (1<<6))                 // If the UART receive is full
    {
       UART0_ICR_R |= (1<<4);               // Reset the RX interrupt
    }
}

/****************************** End Of Module *******************************/
