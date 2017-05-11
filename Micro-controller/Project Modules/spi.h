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

#ifndef SPI_H_
#define SPI_H_

/***************************** Include files *******************************/

#include <stdint.h>
#include "global_def.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "timer0A.h"

/*****************************    Defines    *******************************/
extern INT16U spi_data;

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

extern void init_spi0();
/*****************************************************************************
*   Input    :
*   Output   : -
*   Function : Initialize SSI module2 as Freescale SPI
******************************************************************************/

extern void init_spi2();
/*****************************************************************************
*   Input    :
*   Output   : -
*   Function : Initialize SSI module2 as Freescale SPI
******************************************************************************/


/****************************** End Of Module *******************************/

extern void spi_rx_task(INT8U task_id, INT8U task_state, INT8U event, INT8U data);
/*****************************************************************************
*   Input    :
*   Output   : -
*   Function : Initialize SSI module2 as Freescale SPI
******************************************************************************/

extern void spi_tx_task(INT8U task_id, INT8U task_state, INT8U event, INT8U data);
/*****************************************************************************
*   Input    :
*   Output   : -
*   Function : Initialize SSI module2 as Freescale SPI
******************************************************************************/


/****************************** End Of Module *******************************/

#endif /* SPI_H_ */
