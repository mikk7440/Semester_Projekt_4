/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: emp.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "emp_type.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/
INT16U set_spd_get_cur_motor_1(INT8S speed_val) // speed val is +- 127
{
    INT16U speed1;
    speed1 = speed_val;
    speed1 &= 0xFF;     // Hvorfor
    speed1 |= (1<<12); // bit 14 motor 1, bit 13 motor 2
    return speed1;
}


INT16U get_cur_motor_1() // speed val is +- 127
{
    INT16U speed1 = 0;
    speed1 = (2<<12); // bit 14 motor 1, bit 13 motor 2
    return speed1;
}

INT16U get_spd_motor_1() // speed val is +- 127
{
    INT16U speed1;
    speed1 = (3<<12); // bit 14 motor 1, bit 13 motor 2
    return speed1;
}


INT16U get_enc_motor_1() // speed val is +- 127
{
    INT16U speed1;
    speed1 = (4<<12); // bit 14 motor 1, bit 13 motor 2
    return speed1;
}


INT16U set_spd_get_cur_motor_2(INT8S speed_val) // speed val is +- 127
{
    INT16U speed2;
    speed2 = speed_val;
    speed2 &= 0xFF;     // Hvorfor
    speed2 |= (5<<12); // bit 14 motor 1, bit 13 motor 2
    return speed2;
}

INT16U get_cur_motor_2() // speed val is +- 127
{
    INT16U speed2;
    speed2 = (6<<12); // bit 14 motor 1, bit 13 motor 2
    return speed2;
}

INT16U get_spd_motor_2() // speed val is +- 127
{
    INT16U speed2;
    speed2 = (7<<12); // bit 14 motor 1, bit 13 motor 2
    return speed2;
}

INT16U get_enc_motor_2() // speed val is +- 127
{
    INT16U speed2;
    speed2 = (8<<12); // bit 14 motor 1, bit 13 motor 2
    return speed2;
}

INT16U reset_motor_enc(void)
{
    INT16U reset = (15<<12);
 //   uartWriteStr("reset");
    return reset;
}

INT16U test() // speed val is +- 127
{
    INT16U speed;
    speed = (13<<12); // bit 14 motor 1, bit 13 motor 2
    return speed;
}



/****************************** End Of Module *******************************/












