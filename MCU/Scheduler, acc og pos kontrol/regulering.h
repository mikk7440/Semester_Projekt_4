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
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/


INT8S goTo(INT16U enc, INT16U destination); //distnation is enc val not degrees
INT8S goTo2(INT16U enc, INT16U destination);
INT16U reset_motor_enc(void);
INT16U get_enc_motor_2();
INT16U get_spd_motor_2();
INT16U get_cur_motor_2();
INT16U set_spd_get_cur_motor_2(INT8S speed_val);
INT16U get_enc_motor_1();
INT16U get_spd_motor_1();
INT16U get_cur_motor_1();
INT16U set_spd_get_cur_motor_1(INT8S speed_val);

void reg_m1_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data);
void reg_m2_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data);

/****************************** End Of Module *******************************/
#endif

