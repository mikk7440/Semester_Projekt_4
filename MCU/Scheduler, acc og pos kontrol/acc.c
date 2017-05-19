/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: acc.c
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
#include "messages.h"
#include "timer0A.h"
#include "spi.h"
#include <stdio.h>
#include <math.h>

/*****************************    Defines    *******************************/
#define SEND_SPI         1
#define WAIT_FOR_DATA    2
#define SEND_UART        3
#define SEND_ANGLE       4

#define M_PI             3.14159265358979323846

/*****************************   Constants   *******************************/

INT16U writ_instr =    0b0000000000000000;
INT16U read_instr =   0b1000000000000000;
INT16U adr_acc_x0 = 0b0011001000000000;
INT16U adr_acc_x1 = 0b0011001100000000;
INT16U adr_acc_y0 = 0b0011010000000000;
INT16U adr_acc_y1 = 0b0011010100000000;
INT16U adr_acc_z0 = 0b0011011000000000;
INT16U adr_acc_z1 = 0b0011011100000000;
INT16U get_acc_addr;

/*****************************   Variables   *******************************/


INT16U ch;
INT8U ACC_STATE = SEND_SPI;
INT16U data_acc[6];
INT16U data_acc_total[3];
INT8U sign[3];
INT16U data_send = 0;

INT8U i = 0;
INT8U j = 0;

INT16U v1 = 2000;
INT16U v2 = 2000;

char axis[] = "xyz";


/*****************************   Functions   *******************************/
void acc_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    switch(ACC_STATE)
          {
          case SEND_SPI:
              select_color(RED);
              if(!get_queue( QUEUE_SPI_RX_ACC, &spi_data, WAIT_FOREVER )) //if queue is empty
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
              select_color(BLUE);
              if (i < 6)
              {
                  if( get_queue( QUEUE_SPI_RX_ACC, &spi_data, WAIT_FOREVER ))
                     {
                           data_acc[i] = spi_data;
                           i++;
                     }
              }
              else
              {
                  i = 0;
                  ACC_STATE = SEND_UART;
                  put_queue( QUEUE_UART_TX, '\r', WAIT_FOREVER );
              }
              break;

          case SEND_UART:
              if(j < 6)
              {
                  data_send = ((data_acc[j] & 0b11111111) | ((data_acc[j+1] << 8) & 0b1111111100000000));


                  if( ((data_acc[j+1] << 8) & (1<<15) ) )
                  {
                          data_send = ((~data_send) + 1) *4;
                          data_acc_total[j/2] = data_send;
                          sign[j/2] = 1;

                  }
                  else
                  {
                          data_send = data_send*4;
                          data_acc_total[j/2] = data_send;
                          sign[j/2] = 0;
                  }

                  data_send = data_acc_total[j/2];

                  j = j + 2;
              }
              else
              {
                  j = 0;
                  ACC_STATE = SEND_ANGLE;
              }
              break;

          case SEND_ANGLE:
              select_color(YELLOW);
              if (sign[2] == 0)
              {
                  v1 = atan((double)data_acc_total[0]/data_acc_total[2])*(180/M_PI);

                  if( (sign[2] == sign[0]))
                  {
                      v1 = 2000 - v1*3;
                  }
                  else
                  {
                      v1 = 2000 + v1*3;
                  }

                  v2 = atan((double)data_acc_total[1]/data_acc_total[2])*(180/M_PI);

                  if( (sign[2] == sign[1]))
                  {
                      v2 = 2000 + v2 * 3;
                  }
                  else
                  {
                      v2 = 2000 - v2 * 3;
                  }
              }
              ACC_STATE = SEND_SPI;
              break;

          default:
          break;
          };
 }


/****************************** End Of Module *******************************/
