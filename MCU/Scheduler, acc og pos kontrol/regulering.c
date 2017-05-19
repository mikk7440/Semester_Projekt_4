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
#include "global_def.h"
#include <stdint.h>
#include "swtimers.h"
#include "tm4c123gh6pm.h"
#include "timer0A.h"
#include "gpio.h"
#include "LEDS.h"
#include "uart.h"
#include "rtcs.h"
#include "spi.h"
#include <string.h>
#include "acc.h"
/*****************************    Defines    *******************************/
#define get_enc_motor   1
#define SET_POS         2
#define REG_SEND        3

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

extern INT16U v1;
extern INT16U v2;

/*****************************   Functions   *******************************/
INT16U tToSpeed(INT16U val)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    INT16U calc_val;
    if (val == 0)
    {
        calc_val = 0;
    }
    else
    {
        calc_val = 33333.33/val;
    }
    return calc_val;
}

INT8S goTo(INT16U enc, INT16U destination) //distnation is enc val not degrees
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    //Variable til positions-loop
    static FP32 last_error_pos = 0;         // Forrige fejl
    static FP32 error_pos = 0;              // Fejl i position
    static FP32 integral_pos = 0;           // Integrator position
    static FP32 derivative_pos = 0;         // derivation af position
    static INT16S output_pos = 0;           // Output fra positions-loopet - input til hastighedsloopet
    FP32 iteration_time_pos = 0.005;        // Tiden mellem hver kald til integraleledddet og differentieringen afh�nger af iteration-counter
    FP32 kP_pos = 0.5;                        // position p-gain
    FP32 kI_pos = 15;                       // position i-gain = 15
    FP32 kD_pos = 0.001;                     // position k gain - skal umiddelbart ikke bruges. = 0.001


    // Variable til hastigheds-loop
    static FP32 error_hastighed = 0;        // Fejlen i hastighed
    FP32 kP_hastighed = .4;                 // Position p-gain
    INT16S output_hastighed = 0;            // output fra hastigheds-loopet

    static INT8U iteration_counter = 2;     // Antallet af gange de indre loop skal eksekveres pr. gang de ydre loop

        error_pos = destination - enc;      // Fejl-udregningen
        // Hvis positionen er indenfor +/- 5 s�ttes fejlen til 0:
        if (error_pos < 5 && error_pos > -5)
            error_pos = 0;
        // Hvis fejlen er indenfor +/- 35 aktiveres i-leddet
        if (error_pos < 35 && error_pos > -35)
        {
            integral_pos = integral_pos + (error_pos * iteration_time_pos);
        }
        // differential-leddet som umiddelbart ikke bruges.
        derivative_pos = (error_pos - last_error_pos)/iteration_time_pos;
        last_error_pos = error_pos;         // Anvendes som forrige fejl i n�ste iteration
        // Samlingen af alle led. Bestemmer outputtet af positions-loopet:
        output_pos = kP_pos * error_pos + kI_pos * integral_pos + kD_pos * derivative_pos;
        // Hvis fejlen bliver nul( som det g�r indenfor +/- 5, nulstilles integrale-leddet, s� der ikke skal et
        // overshoot til for at f� det til 0:
        if ( error_pos == 0)
        {
            integral_pos = 0;
        }
        // Begr�nsningen af hastigheden. Max-hastigheden er cirka 400. Hvis der kun reguleres p� position, skal det
        // �ndres til +/- 127, s� der ikke skal overflow til PWM. Hvis der skal s�ttes en maksimum hastighed
        // skal det g�res her.

        if (output_pos > 127)
        {
            output_pos = 127;
        }
        else if (output_pos < -127)
        {
            output_pos = -127;
        }
    return output_pos;
}


INT8S goTo2(INT16U enc, INT16U destination) //distnation is enc val not degrees
{
    //Variable til positions-loop
    static FP32 last_error_pos = 0;         // Forrige fejl
    static FP32 error_pos = 0;              // Fejl i position
    static FP32 integral_pos = 0;           // Integrator position
    static FP32 derivative_pos = 0;         // derivation af position
    static INT16S output_pos = 0;           // Output fra positions-loopet - input til hastighedsloopet
    FP32 iteration_time_pos = 0.005;        // Tiden mellem hver kald til integraleledddet og differentieringen afh�nger af iteration-counter
    FP32 kP_pos = 0.5;                        // position p-gain
    FP32 kI_pos = 1;                       // position i-gain = 1
    FP32 kD_pos = 0.0001;                        // position k gain - skal umiddelbart ikke bruges. = 0.0001


    // Variable til hastigheds-loop
    static FP32 error_hastighed = 0;        // Fejlen i hastighed
    FP32 kP_hastighed = .4;                 // Position p-gain
    INT16S output_hastighed = 0;            // output fra hastigheds-loopet

    static INT8U iteration_counter = 2;     // Antallet af gange de indre loop skal eksekveres pr. gang de ydre loop

        error_pos = destination - enc;      // Fejl-udregningen
        // Hvis positionen er indenfor +/- 5 s�ttes fejlen til 0:
        if (error_pos < 5 && error_pos > -5)
            error_pos = 0;
        // Hvis fejlen er indenfor +/- 35 aktiveres i-leddet
        if (error_pos < 35 && error_pos > -35)
        {
            integral_pos = integral_pos + (error_pos * iteration_time_pos);
        }
        // differential-leddet som umiddelbart ikke bruges.
        derivative_pos = (error_pos - last_error_pos)/iteration_time_pos;
        last_error_pos = error_pos;         // Anvendes som forrige fejl i n�ste iteration
        // Samlingen af alle led. Bestemmer outputtet af positions-loopet:
        output_pos = kP_pos * error_pos + kI_pos * integral_pos + kD_pos * derivative_pos;
        // Hvis fejlen bliver nul( som det g�r indenfor +/- 5, nulstilles integrale-leddet, s� der ikke skal et
        // overshoot til for at f� det til 0:
        if ( error_pos == 0)
        {
            integral_pos = 0;
        }
        // Begr�nsningen af hastigheden. Max-hastigheden er cirka 400. Hvis der kun reguleres p� position, skal det
        // �ndres til +/- 127, s� der ikke skal overflow til PWM. Hvis der skal s�ttes en maksimum hastighed
        // skal det g�res her.

        if (output_pos > 127)
        {
            output_pos = 127;
        }
        else if (output_pos < -127)
        {
            output_pos = -127;
        }
    return output_pos;
}

INT16U set_spd_get_cur_motor_1(INT8S speed_val) // speed val is +- 127
{
    INT16U speed1;
    speed1 = speed_val;
    speed1 &= 0xFF;     // Hvorfor
    speed1 |= (1<<13); // bit 14 motor 1, bit 13 motor 2
    return speed1;
}


//INT16U get_cur_motor_1() // speed val is +- 127
//{
//    INT16U speed1 = 0;
//    speed1 = (2<<12); // bit 14 motor 1, bit 13 motor 2
//    return speed1;
//}
//
//INT16U get_spd_motor_1() // speed val is +- 127
//{
//    INT16U speed1;
//    speed1 = (3<<12); // bit 14 motor 1, bit 13 motor 2
//    return speed1;
//}


INT16U get_enc_motor_1() // speed val is +- 127
{
    INT16U speed1;
    speed1 = (2<<13); // bit 14 motor 1, bit 13 motor 2
    return speed1;
}


INT16U set_spd_get_cur_motor_2(INT8S speed_val) // speed val is +- 127
{
    INT16U speed2;
    speed2 = speed_val;
    speed2 &= 0xFF;     // Hvorfor
    speed2 |= (3<<13); // bit 14 motor 1, bit 13 motor 2
    return speed2;
}

//INT16U get_cur_motor_2() // speed val is +- 127
//{
//    INT16U speed2;
//    speed2 = (6<<12); // bit 14 motor 1, bit 13 motor 2
//    return speed2;
//}
//
//INT16U get_spd_motor_2() // speed val is +- 127
//{
//    INT16U speed2;
//    speed2 = (7<<12); // bit 14 motor 1, bit 13 motor 2
//    return speed2;
//}

INT16U get_enc_motor_2() // speed val is +- 127
{
    INT16U speed2;
    speed2 = (4<<13); // bit 14 motor 1, bit 13 motor 2
    return speed2;
}

INT16U reset_motor_enc(void)
{
    INT16U reset = (15<<12);
 //   uartWriteStr("reset");
    return reset;
}



void reg_m1_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    static INT8U reg_m1_STATE = get_enc_motor;
    static BOOLEAN recive_speed    = TRUE;

    static INT16U current_m1     = 0;
    static INT16U speed_m1       = 0;
    static INT16U position_m1    = 0;

    INT8U var;

    switch(reg_m1_STATE)
        {
        case get_enc_motor:

            put_queue(QUEUE_SPI_TX_FPGA,get_enc_motor_1(),WAIT_FOREVER);
            reg_m1_STATE = SET_POS;

        break;
        case SET_POS:
            if (!peek_queue(QUEUE_SPI_RX_FPGA))
            {
                get_queue(QUEUE_SPI_RX_FPGA, &spi_data_FPGA, WAIT_FOREVER);
            }

            if ( !(peek_queue(QUEUE_SPI_RX_FPGA) & (1<<12)) && get_queue(QUEUE_SPI_RX_FPGA, &spi_data_FPGA, WAIT_FOREVER) )
            {
                position_m1 = spi_data_FPGA;
                reg_m1_STATE = REG_SEND;
            }

        break;

        case REG_SEND:
//            put_queue(QUEUE_SPI_TX_FPGA, set_spd_get_cur_motor_1(-64), WAIT_FOREVER);
            put_queue(QUEUE_SPI_TX_FPGA, set_spd_get_cur_motor_1(goTo(position_m1,v1)), WAIT_FOREVER);
            reg_m1_STATE = get_enc_motor;
        break;

        }
}

void reg_m2_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    static INT8U reg_m2_STATE = get_enc_motor;

    static BOOLEAN recive_speed    = TRUE;

    static INT16U position_m2    = 0;


    INT16U var;

    switch(reg_m2_STATE)
        {
        case get_enc_motor:
            select_color(GREEN);
            put_queue(QUEUE_SPI_TX_FPGA,get_enc_motor_2(),WAIT_FOREVER);
            reg_m2_STATE = SET_POS;

        break;
        case SET_POS:
            var = peek_queue(QUEUE_SPI_RX_FPGA);
            if (! ( peek_queue(QUEUE_SPI_RX_FPGA) & ~(1<<12) ))
            {
                select_color(RED);
                get_queue(QUEUE_SPI_RX_FPGA, &spi_data_FPGA, WAIT_FOREVER);
            }
            if ( (peek_queue(QUEUE_SPI_RX_FPGA) & (1<<12)) && get_queue(QUEUE_SPI_RX_FPGA, &spi_data_FPGA, WAIT_FOREVER) )
            {
                select_color(BLUE);
                position_m2 = spi_data_FPGA & ~(1<<12);
                reg_m2_STATE = REG_SEND;
            }

        break;

        case REG_SEND:
            put_queue(QUEUE_SPI_TX_FPGA, set_spd_get_cur_motor_2(goTo2(position_m2,v2)), WAIT_FOREVER);
            reg_m2_STATE = get_enc_motor;
        break;

        }
}


/****************************** End Of Module *******************************/












