#include "global_def.h"
#include <stdint.h>
#include "swtimers.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "timer0A.h"
#include "gpio.h"
#include "LEDS.h"
#include "button.h"
#include "uart.h"
#include "rtcs.h"
#include "spi.h"
#include <string.h>

INT8U i = 0;
INT8U  InBuf[128];

INT16U ch;
INT16U writ_instr =    0b0000000000000000;
INT16U read_instr =   0b1000000000000000;
INT16U adr_acc_x0 = 0b0011001000000000;
INT16U adr_acc_x1 = 0b0011001100000000;
INT16U adr_acc_y0 = 0b0011010000000000;
INT16U adr_acc_y1 = 0b0011010100000000;
INT16U adr_acc_z0 = 0b0011011000000000;
INT16U adr_acc_z1 = 0b0011011100000000;
INT16U get_acc_addr;

#define IDLE                    1
#define SEND_SPI            2
#define WAIT                    3
#define SEND_UART       5




/*****************************   Functions   *******************************/


void ui_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    static INT8U ACC_STATE = IDLE;
    static INT16U data_acc[6];
    static INT16U data_send = 0;
    char axis[] = "xyz";

    static INT8U i = 0;
    static INT8U j = 0;


    switch(ACC_STATE)
          {
    case IDLE:
              if( get_queue( QUEUE_UART_RX, &ch, WAIT_FOREVER ) )
              {
                  ACC_STATE = SEND_SPI;
               }
              break;

          case SEND_SPI:
              get_acc_addr = read_instr | adr_acc_x0;
              put_queue( QUEUE_SPI_TX_0, get_acc_addr, WAIT_FOREVER );
              get_acc_addr = read_instr | adr_acc_x1;
              put_queue( QUEUE_SPI_TX_0, get_acc_addr, WAIT_FOREVER );
              get_acc_addr = read_instr | adr_acc_y0;
              put_queue( QUEUE_SPI_TX_0, get_acc_addr, WAIT_FOREVER );
              get_acc_addr = read_instr | adr_acc_y1;
              put_queue( QUEUE_SPI_TX_0, get_acc_addr, WAIT_FOREVER );
              get_acc_addr = read_instr | adr_acc_z0;
              put_queue( QUEUE_SPI_TX_0, get_acc_addr, WAIT_FOREVER );
              get_acc_addr = read_instr | adr_acc_z1;
              put_queue( QUEUE_SPI_TX_0, get_acc_addr, WAIT_FOREVER );

              ACC_STATE = WAIT;
              break;

          case WAIT:
              if (i < 6)
              {
                  if( get_queue( QUEUE_SPI_RX_0, &spi_data, WAIT_FOREVER ))
                     {
                               data_acc[i] = spi_data;
                               i++;
                     }
              }else{
                  i = 0;
                  ACC_STATE = SEND_UART;
                  put_queue( QUEUE_UART_TX, '\r', WAIT_FOREVER );
              }
              break;

          case SEND_UART:
              if(j < 6)
              {

                  put_queue( QUEUE_UART_TX, axis[j/2],  WAIT_FOREVER );
                  put_queue( QUEUE_UART_TX, ':',  WAIT_FOREVER );
                  put_queue( QUEUE_UART_TX, ' ',  WAIT_FOREVER );

                  if( ((data_acc[j+1] << 8) & 0b0000001000000000) )
                  {
                          put_queue( QUEUE_UART_TX, '-',  WAIT_FOREVER );
                          data_send = ((data_acc[j] & 0b11111111) | ((data_acc[j+1] << 8) & 0b0000001100000000))*4;

                  }else{
                          put_queue( QUEUE_UART_TX, ' ' , WAIT_FOREVER);
                          data_send = ((data_acc[j] & 0b11111111) | ((data_acc[j+1] << 8) & 0b0000001100000000))*4;
                  }


                  put_queue( QUEUE_UART_TX, (data_send/1000)%10+48, WAIT_FOREVER);
                  put_queue( QUEUE_UART_TX, (data_send/100)%10+48, WAIT_FOREVER);
                  put_queue( QUEUE_UART_TX, (data_send/10)%10+48, WAIT_FOREVER);
                  put_queue( QUEUE_UART_TX, (data_send)%10+48, WAIT_FOREVER);
                  put_queue( QUEUE_UART_TX, ',',  WAIT_FOREVER );
                  put_queue( QUEUE_UART_TX, ' ',  WAIT_FOREVER );


                  j = j + 2;
              }else{
                  j = 0;
                  ACC_STATE = IDLE;
              }
              break;

          default:
          break;
          };

 }


int main()
/*****************************************************************************
*   Input    : NONE
*   Output   : Returns 0
*   Function : Init hardware and then loop forever
******************************************************************************/
{
    init_spi0();
    init_gpio();
    init_uart();
//    init_spi2();
    init_timer0a(5);    // 5 ms timer value
    init_timers();

    init_rtcs();

    open_queue(QUEUE_UART_RX);
    open_queue(QUEUE_UART_TX);
    open_queue(QUEUE_SPI_RX_0);
    open_queue(QUEUE_SPI_TX_0);

    start_task(TASK_UART_RX, uart_rx_task);
    start_task(TASK_UART_TX, uart_tx_task);
    start_task(TASK_SPI_TX_0, spi_tx_task);
    start_task(TASK_SPI_RX_0, spi_rx_task);
    start_task(TASK_UI, ui_task);


    schedule();

  return 0;
}
