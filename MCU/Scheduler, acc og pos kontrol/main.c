#include <stdint.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "global_def.h"
#include "emp_type.h"
#include "80_MHz.h"
#include "swtimers.h"
#include "timer0A.h"
#include "gpio.h"
#include "LEDS.h"
#include "uart.h"
#include "rtcs.h"
#include "spi.h"
#include "regulering.h"
#include "acc.h"
#include <stdio.h>
#include <math.h>


/*****************************   Functions   *******************************/

int main()
/*****************************************************************************
*   Input    : NONE
*   Output   : Returns 0
*   Function : Init hardware and then loop forever
******************************************************************************/
{
    init_80_MHz();
    init_spi0();


    init_gpio();
    init_uart();
    init_timer0a(5000);    // 5 us timer value Alternative for systik
    init_timers();

    init_spi2();

    init_rtcs();


    /*  QUEUE  */
    open_queue(QUEUE_UART_RX);
    open_queue(QUEUE_UART_TX);

    open_queue(QUEUE_SPI_RX_ACC);
    open_queue(QUEUE_SPI_TX_ACC);

    open_queue(QUEUE_SPI_RX_FPGA);
    open_queue(QUEUE_SPI_TX_FPGA);


    /*  TASK  */
    start_task(TASK_UART_RX, uart_rx_task);
    start_task(TASK_UART_TX, uart_tx_task);

    start_task(TASK_SPI_TX_ACC, spi_tx_task_acc);
    start_task(TASK_SPI_RX_ACC, spi_rx_task_acc);

    start_task(TASK_SPI_TX_FPGA, spi_tx_task_FPGA);
    start_task(TASK_SPI_RX_FPGA, spi_rx_task_FPGA);

    start_task(TASK_ACC, acc_task);

    start_task(TASK_REG_M1,reg_m1_task );
    start_task(TASK_REG_M2,reg_m2_task );

    /*  schedule  */
    schedule();

  return 0;
}
