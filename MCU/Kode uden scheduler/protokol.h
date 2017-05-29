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

#ifndef _PROTOKOL_H
  #define _PROTOKOL_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/********************** External declaration of Variables ******************/

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/
INT16U set_spd_get_cur_motor_1(INT8S speed_val); // speed val is +- 127

INT16U get_cur_motor_1(); // speed val is +- 127

INT16U get_spd_motor_1(); // speed val is +- 127

INT16U get_enc_motor_1(); // speed val is +- 127

INT16U set_spd_get_cur_motor_2(INT8S speed_val); // speed val is +- 127

INT16U get_cur_motor_2(); // speed val is +- 127

INT16U get_spd_motor_2(); // speed val is +- 127

INT16U get_enc_motor_2(); // speed val is +- 127

INT16U reset_motor_enc(void);

INT16U test(); // speed val is +- 127



/****************************** End Of Module *******************************/
#endif


