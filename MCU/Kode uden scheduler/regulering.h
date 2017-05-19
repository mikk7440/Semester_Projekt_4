/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: emp.h
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
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef _REGULERING_H
  #define _REGULERING_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/********************** External declaration of Variables ******************/

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/

INT16U tToSpeed(INT16U val);
INT16U tToSpeed(INT16U val);
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/


INT8S goTo(INT16U enc, INT16U destination, INT16U hastighed); //distnation is enc val not degrees
INT8S goTo2(INT16U enc, INT16U destination, INT16U hastighed); //distnation is enc val not degrees

/****************************** End Of Module *******************************/
#endif

