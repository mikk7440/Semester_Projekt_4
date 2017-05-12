/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: uart.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150228  MoH   Module created.
*
*****************************************************************************/

#ifndef _UART_H
  #define _UART_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

extern BOOLEAN uart0_rx_rdy();
/*****************************************************************************
*   Input    : -
*   Output   : TRUE if rx-buffer ready
*   Function : Character ready at uart0 RX
******************************************************************************/

extern INT8U uart0_getc();
/*****************************************************************************
*   Input    : -
*   Output   : Character received
*   Function : Get character from uart0 RX
******************************************************************************/

extern BOOLEAN uart0_tx_rdy();
/*****************************************************************************
*   Input    : -
*   Output   : TRUE, if tx-buffer ready
*   Function : uart0 TX buffer ready
******************************************************************************/

extern void uart0_putc( INT8U );
/*****************************************************************************
*   Input    : Character to be send
*   Output   : -
*   Function : Put character to uart0 TX
******************************************************************************/





INT16S uartReadChar(void);


void uartWriteChar(INT16U c);

void uartWriteStr(INT8U *character);

void UART_OutUDec(INT32U n);






extern void init_uart0( INT32U, INT8U, INT8U, INT8U );
/*****************************************************************************
*   Input    : Baud_rate
*              Databits: 5, 6, 7 or 8(default)
*              Stop bits: 1(default) or 2
*              Parity: 'e', 'o', '0', '1' or 'n'(default)
*   Output   : -
*   Function : Initialize uart 0
******************************************************************************/


/****************************** End Of Module *******************************/
#endif
