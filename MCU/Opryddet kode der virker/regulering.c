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

INT8U    dummy1;
INT16S   dummy2;

/*****************************   Functions   *******************************/
FP64 tToSpeed(INT16U val)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    INT16U calc_val;
    static INT16U avg1 = 0;
    static INT16U avg2 = 0;
    static INT16U avg3 = 0;
    static INT16U avg4 = 0;
    static INT16U avg5 = 0;
    static INT16U avg6 = 0;
    if (val == 0)
    {
        calc_val = 0;
    }
    else
    {
        calc_val = 33333.33/val;
    }
    avg6 = avg5;
    avg5 = avg4;
    avg4 = avg3;
    avg3 = avg2;
    avg2=avg1;
    avg1=calc_val;
    calc_val = (avg1+avg2+avg3+avg4+avg5+avg6)/6;

    return calc_val;
}

INT8S goTo(INT16U enc, INT16U destination, INT16U hastighed) //distnation is enc val not degrees
{
    static FP32 last_error_pos = 0;
    static FP32 error_pos = 0;
    FP32 iteration_time_pos = 0.005;
    static FP32 integral_pos = 0;
    static FP32 derivative_pos = 0;
    FP32 kP_pos = 1;//2;
    FP32 kI_pos = 0;//10;
    FP32 kD_pos = 0;
    static INT16S output_pos = 0;


    static FP32 error_hastighed = 0;
    FP32 kP_hastighed = .4;
    INT16S output_hastighed = 0;

    static INT8U iteration_counter = 2;


//    if(iteration_counter == 2)
//      {
        error_pos = destination - enc;

//        if (error_pos < 5 && error_pos > -5)
//            error_pos = 0;
//        if (error_pos < 35 && error_pos > -35)
//        {
//            integral_pos = integral_pos + (error_pos * iteration_time_pos);
//        }


        derivative_pos = (error_pos - last_error_pos)/iteration_time_pos;
        last_error_pos = error_pos;

        output_pos = (kP_pos * error_pos);// + kI_pos * integral_pos + kD_pos * derivative_pos);

//        if ( error_pos == 0)
//        {
//            integral_pos = 0;
//        }

        if (output_pos > 121)
        {
            output_pos = 121;
        }
        else if (output_pos < -121)
        {
            output_pos = -121;
        }
//        iteration_counter=0;
//      }

    error_hastighed = output_pos - hastighed;
    output_hastighed = error_hastighed * kP_hastighed;
//    iteration_counter++;
        if (output_hastighed > 127)
        {
            output_hastighed = 127;
        }
        else if (output_hastighed < -127)
        {
            output_hastighed = -127;
        }

    return output_pos;
}

/****************************** End Of Module *******************************/












