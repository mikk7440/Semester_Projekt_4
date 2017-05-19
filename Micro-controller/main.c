#include <stdio.h>
#include <math.h>
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

#define IDLE                           1
#define SEND_SPI                  2
#define WAIT_FOR_DATA       3
#define SEND_UART              4
#define SEND_ANGLE            5

#define M_PI                            3.14159265358979323846




/*****************************   Functions   *******************************/
void acc_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    static INT8U ACC_STATE = IDLE;
    static INT16U data_acc[6];
    static INT16U data_acc_total[3];
    static INT8U sign[3];
    static INT16U data_send = 0;
    char axis[] = "xyz";

    static INT8U i = 0;
    static INT8U j = 0;

    static INT16U v1 = 0;
    static INT16U v2 = 0;



    switch(ACC_STATE)
          {
           case IDLE:
    //          if( get_queue( QUEUE_UART_RX, &ch, WAIT_FOREVER ) )
    //          {
    //              ACC_STATE = SEND_SPI;
    //           }
              break;

          case SEND_SPI:
              if(!get_queue( QUEUE_SPI_RX_ACC, &spi_data, WAIT_FOREVER ))
              {
              get_acc_addr = read_instr | adr_acc_x0;
              put_queue( QUEUE_SPI_TX_ACC, get_acc_addr, WAIT_FOREVER );
              get_acc_addr = read_instr | adr_acc_x1;
              put_queue( QUEUE_SPI_TX_ACC, get_acc_addr, WAIT_FOREVER );
              get_acc_addr = read_instr | adr_acc_y0;
              put_queue( QUEUE_SPI_TX_ACC, get_acc_addr, WAIT_FOREVER );
              get_acc_addr = read_instr | adr_acc_y1;
              put_queue( QUEUE_SPI_TX_ACC, get_acc_addr, WAIT_FOREVER );
              get_acc_addr = read_instr | adr_acc_z0;
              put_queue( QUEUE_SPI_TX_ACC, get_acc_addr, WAIT_FOREVER );
              get_acc_addr = read_instr | adr_acc_z1;
              put_queue( QUEUE_SPI_TX_ACC, get_acc_addr, WAIT_FOREVER );

              ACC_STATE = WAIT_FOR_DATA;
              };
              break;

          case WAIT_FOR_DATA:
              if (i < 6)
              {
                  if( get_queue( QUEUE_SPI_RX_ACC, &spi_data, WAIT_FOREVER ))
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

                  data_send = ((data_acc[j] & 0b11111111) | ((data_acc[j+1] << 8) & 0b1111111100000000));


                  if( ((data_acc[j+1] << 8) & (1<<15) ) )
                  {
                          put_queue( QUEUE_UART_TX, '-',  WAIT_FOREVER );
                          data_send = ((~data_send) + 1) *4;
                          data_acc_total[j/2] = data_send;
                          sign[j/2] = 1;

                  }else{
                          put_queue( QUEUE_UART_TX, ' ' , WAIT_FOREVER);
                          data_send = data_send*4;
                          data_acc_total[j/2] = data_send;
                          sign[j/2] = 0;
                  }

                  data_send = data_acc_total[j/2];

                  put_queue( QUEUE_UART_TX, (data_send/1000)%10+48, WAIT_FOREVER);
                  put_queue( QUEUE_UART_TX, (data_send/100)%10+48, WAIT_FOREVER);
                  put_queue( QUEUE_UART_TX, (data_send/10)%10+48, WAIT_FOREVER);
                  put_queue( QUEUE_UART_TX, (data_send)%10+48, WAIT_FOREVER);
                  put_queue( QUEUE_UART_TX, ',',  WAIT_FOREVER );
                  put_queue( QUEUE_UART_TX, ' ',  WAIT_FOREVER );

                  j = j + 2;
              }else{
                  j = 0;
                  ACC_STATE = SEND_ANGLE;
              }
              break;

          case SEND_ANGLE:

              v1 = atan((double)data_acc_total[0]/data_acc_total[2])*(180/M_PI);
              put_queue( QUEUE_UART_TX, 'v',  WAIT_FOREVER );
              put_queue( QUEUE_UART_TX, '1',  WAIT_FOREVER );
              put_queue( QUEUE_UART_TX, ':',  WAIT_FOREVER );
              put_queue( QUEUE_UART_TX, ' ',  WAIT_FOREVER );

              if( (sign[2] == sign[0]))
              {
                  v1 = 2000 + v1*3;
                //  put_queue( QUEUE_UART_TX, '0',  WAIT_FOREVER );
              }else{
                  v1 = 2000 - v1*3;
              //    put_queue( QUEUE_UART_TX, '-',  WAIT_FOREVER );
              }

              put_queue( QUEUE_UART_TX, (v1/1000)%10+48, WAIT_FOREVER);
              put_queue( QUEUE_UART_TX, (v1/100)%10+48, WAIT_FOREVER);
              put_queue( QUEUE_UART_TX, (v1/10)%10+48, WAIT_FOREVER);
              put_queue( QUEUE_UART_TX, (v1)%10+48, WAIT_FOREVER);
              put_queue( QUEUE_UART_TX, ',',  WAIT_FOREVER );
              put_queue( QUEUE_UART_TX, ' ',  WAIT_FOREVER );


              v2 = atan((double)data_acc_total[1]/data_acc_total[2])*(180/M_PI) ;
              put_queue( QUEUE_UART_TX, 'v',  WAIT_FOREVER );
              put_queue( QUEUE_UART_TX, '2',  WAIT_FOREVER );
              put_queue( QUEUE_UART_TX, ':',  WAIT_FOREVER );
              put_queue( QUEUE_UART_TX, ' ',  WAIT_FOREVER );

              if( (sign[2] == sign[1]))
              {
                  v2 = 2000 + v2 * 3;
              }else{
                  v2 = 2000 - v2 * 3;
              }

              put_queue( QUEUE_UART_TX, (v2/1000)%10+48, WAIT_FOREVER);
              put_queue( QUEUE_UART_TX, (v2/100)%10+48, WAIT_FOREVER);
              put_queue( QUEUE_UART_TX, (v2/10)%10+48, WAIT_FOREVER);
              put_queue( QUEUE_UART_TX, (v2)%10+48, WAIT_FOREVER);
              put_queue( QUEUE_UART_TX, ',',  WAIT_FOREVER );
              put_queue( QUEUE_UART_TX, ' ',  WAIT_FOREVER );

              ACC_STATE = IDLE;
              break;

          default:
          break;
          };
 }

#define START_MOTOR         4
#define CALCULATE             2
#define GET_DATA            3

double kalman_gain = 0;
double error_estimate = 1;
double error_measure = 1;
double current_estimate = 2000;
double previous_estimate = 2400;
double measurement = 0;
INT8U FILTER_STATE = START_MOTOR;

INT16U current_addr_1 = 0b0010000000000000;
INT16U current_addr_2 = 0b0110000000000000;
INT16U start_motor =       0b0001000001010000;

void ui_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{

 switch (FILTER_STATE)
 {

 case START_MOTOR:
     if( get_queue(QUEUE_UART_RX, &ch, WAIT_FOREVER))
     {
         if(ch == ' ')
         {
             put_queue(QUEUE_SPI_TX_FPGA, start_motor, WAIT_FOREVER);
             FILTER_STATE = IDLE;
         }
     }
     break;

 case IDLE:
     if( get_queue(QUEUE_UART_RX, &ch, WAIT_FOREVER))
     {
         put_queue(QUEUE_UART_TX, ((INT16U)current_estimate/1000)%10+48, WAIT_FOREVER);
         put_queue(QUEUE_UART_TX, ((INT16U)current_estimate/100)%10+48, WAIT_FOREVER);
         put_queue(QUEUE_UART_TX, ((INT16U)current_estimate/10)%10+48, WAIT_FOREVER);
         put_queue(QUEUE_UART_TX, ((INT16U)current_estimate)%10+48, WAIT_FOREVER);
         put_queue(QUEUE_UART_TX, ' ', WAIT_FOREVER);
         put_queue(QUEUE_UART_TX, ',', WAIT_FOREVER);

         put_queue(QUEUE_UART_TX, (spi_data/1000)%10+48, WAIT_FOREVER);
         put_queue(QUEUE_UART_TX, (spi_data/100)%10+48, WAIT_FOREVER);
         put_queue(QUEUE_UART_TX, (spi_data/10)%10+48, WAIT_FOREVER);
         put_queue(QUEUE_UART_TX, (spi_data)%10+48, WAIT_FOREVER);
         put_queue(QUEUE_UART_TX, '\n', WAIT_FOREVER);
         put_queue(QUEUE_UART_TX, '\r', WAIT_FOREVER);
         FILTER_STATE = GET_DATA;
     }else{
     FILTER_STATE = GET_DATA;
     }
     break;

 case GET_DATA:
     if(put_queue(QUEUE_SPI_TX_FPGA, current_addr_1, WAIT_FOREVER))
     {
         FILTER_STATE = CALCULATE;
     }
     break;

     case CALCULATE:
         if(get_queue(QUEUE_SPI_RX_FPGA, &spi_data, WAIT_FOREVER))
             {
             measurement = spi_data;
             kalman_gain = error_estimate / (error_estimate+error_measure);
             current_estimate = previous_estimate + kalman_gain * (measurement - previous_estimate);
             error_estimate = (1 - kalman_gain)*error_estimate;
             previous_estimate = current_estimate;

             FILTER_STATE = IDLE;
             }
         break;


 }


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
    init_spi2();
    init_timer0a(5);    // 5 ms timer value
    init_timers();

    init_rtcs();

    open_queue(QUEUE_UART_RX);
    open_queue(QUEUE_UART_TX);
    open_queue(QUEUE_SPI_RX_ACC);
    open_queue(QUEUE_SPI_TX_ACC);
    open_queue(QUEUE_SPI_RX_FPGA);
    open_queue(QUEUE_SPI_TX_FPGA);


    start_task(TASK_UART_RX, uart_rx_task);
    start_task(TASK_UART_TX, uart_tx_task);
    start_task(TASK_SPI_TX_ACC, spi_tx_task_acc);
    start_task(TASK_SPI_RX_ACC, spi_rx_task_acc);
    start_task(TASK_SPI_TX_FPGA, spi_tx_task_fpga);
    start_task(TASK_SPI_RX_FPGA, spi_rx_task_fpga);
    start_task(TASK_ACC, acc_task);
    start_task(TASK_UI, ui_task);


    schedule();

  return 0;
}
