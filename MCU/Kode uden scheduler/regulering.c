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
INT16U tToSpeed(INT16U val)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
    INT16U calc_val;
    static INT16U avg_1 = 0;
    static INT16U avg_2 = 0;
    static INT16U avg_3 = 0;
    static INT16U avg_4 = 0;
    static INT16U avg_5 = 0;
    static INT16U avg_6 = 0;
    static INT16U avg_7 = 0;
    static INT16U avg_8 = 0;

    if (val == 0)
    {
        calc_val = 0;
    }
    else
    {
        calc_val = 33333.33/val;
    }
    avg_8 = avg_7;
    avg_7 = avg_6;
    avg_6 = avg_5;
    avg_5 = avg_4;
    avg_4 = avg_3;
    avg_3 = avg_2;
    avg_2 = avg_1;
    avg_1 = calc_val;
    return calc_val;//(avg_1 + avg_2 + avg_3 + avg_4 + avg_5 + avg_6 + avg_7 + avg_8)/8;
}
INT16U tToSpeed2(INT16U val)
{
    INT16U calc_val;
    static INT16U avg_1 = 0;
    static INT16U avg_2 = 0;
    static INT16U avg_3 = 0;
    static INT16U avg_4 = 0;

    if (val == 0)
    {
        calc_val = 0;
    }
    else
    {
        calc_val = 33333.33/val;
    }
    avg_4 = avg_3;
    avg_3 = avg_2;
    avg_2 = avg_1;
    avg_1 = calc_val;

    return (avg_1 + avg_2 + avg_3 + avg_4)/4;
}

INT8S goTo(INT16U enc, INT16U destination, INT16U hastighed, INT16S cur) //distnation is enc val not degrees
{

    UART_OutUDec(hastighed);
    uartWriteChar('\n');
    uartWriteChar('\r');
    //Variable til positions-loop
    static FP32 last_error_pos = 0;         // Forrige fejl
    static FP32 error_pos = 0;              // Fejl i position
    static FP32 integral_pos = 0;           // Integrator position
    static FP32 derivative_pos = 0;         // derivation af position
    static INT16S output_pos = 0;  // Output fra positions-loopet - input til hastighedsloopet
    static INT16U last_enc = 0;
    static INT8S direction = 1;
    FP32 iteration_time_pos = 0.005;        // Tiden mellem hver kald til integraleledddet og differentieringen afhænger af iteration-counter
    FP32 kP_pos = 2;                        // position p-gain
    FP32 kI_pos = 30;                       // position i-gain
    FP32 kD_pos = 0.05;                        // position k gain - skal umiddelbart ikke bruges.



    // Variable til hastigheds-loop
    static FP32 error_hastighed = 0;        // Fejlen i hastighed
    FP32 kP_hastighed = .5;                 // Position p-gain
    INT16S output_hastighed = 0;            // output fra hastigheds-loopet
    static INT8U iteration_counter = 2;     // Antallet af gange de indre loop skal eksekveres pr. gang de ydre loop

//    Variable til strøm-loopet:
    static FP32 error_current = 0;
    FP32 kP_current = .7;
    FP32 kI_current = 0;
    INT16S output_current = 0;
    FP32 iteration_time_current = 0;


    if(enc < last_enc)
        direction = -1;
    else
        direction = 1;
    last_enc = enc;


    // Positions-loopet:
//    if(iteration_counter == 2)
//      {
        error_pos = destination - enc;      // Fejl-udregningen
    // Hvis positionen er indenfor +/- 5 sættes fejlen til 0:
        if (error_pos < 5 && error_pos > -5)
            error_pos = 0;
    // Hvis fejlen er indenfor +/- 35 aktiveres i-leddet
        if (error_pos < 35 && error_pos > -35)
        {
            integral_pos = integral_pos + (error_pos * iteration_time_pos);
        }
    // differential-leddet som umiddelbart ikke bruges.
        derivative_pos = (error_pos - last_error_pos)/iteration_time_pos;
        last_error_pos = error_pos;         // Anvendes som forrige fejl i næste iteration
        // Samlingen af alle led. Bestemmer outputtet af positions-loopet:
        output_pos = kP_pos * error_pos + kI_pos * integral_pos + kD_pos * derivative_pos;
        // Hvis fejlen bliver nul( som det gør indenfor +/- 5, nulstilles integrale-leddet, så der ikke skal et
        // overshoot til for at få det til 0:
        if ( error_pos == 0)
        {
            integral_pos = 0;
        }
        // Begrænsningen af hastigheden. Max-hastigheden er cirka 400. Hvis der kun reguleres på position, skal det
        // ændres til +/- 127, så der ikke sker overflow til PWM. Hvis der skal sættes en maksimum hastighed
        // skal det gøres her.

        if (output_pos > 127)
        {
            output_pos = 127;
        }
        else if (output_pos < -127)
        {
            output_pos = -127;
        }
 //       iteration_counter=0;                // Hver gang dette loop køres, skal iterations-counter nulstilles.
 //     }
    // Hastigheds-loopet:
    error_hastighed = output_pos - hastighed*direction;
    output_hastighed = error_hastighed * kP_hastighed;
 //   iteration_counter++;

        if (output_hastighed > 127)
        {
            output_hastighed = 127;
        }
        else if (output_hastighed < -127)
        {
            output_hastighed = -127;
        }
//     Strøm-loop: maks output fra strøm-sensor: 650
        if (cur > 65)
        {
            cur = 0;
        }
        else if (cur < -65)
        {
            cur = 0;
        }

        error_current = output_hastighed - cur;
        output_current = error_current * kP_current;


                if (output_current > 127)
                {
                    output_current = 127;
                }
                else if (output_current < -127)
                {
                    output_current = -127;
                }

        return output_pos;
}


INT8S goTo2(INT16U enc, INT16U destination, INT16U hastighed) //distnation is enc val not degrees
{
    //Variable til positions-loop
    static FP32 last_error_pos = 0;         // Forrige fejl
    static FP32 error_pos = 0;              // Fejl i position
    static FP32 integral_pos = 0;           // Integrator position
    static FP32 derivative_pos = 0;         // derivation af position
    static INT16S output_pos = 0;           // Output fra positions-loopet - input til hastighedsloopet
    FP32 iteration_time_pos = 0.005;        // Tiden mellem hver kald til integraleledddet og differentieringen afhænger af iteration-counter
    FP32 kP_pos = 1.5;                      // position p-gain
    FP32 kI_pos = 30;                       // position i-gain
    FP32 kD_pos = 0;                        // position k gain - skal umiddelbart ikke bruges.

    // Variable til hastigheds-loop
    static FP32 error_hastighed = 0;        // Fejlen i hastighed
    FP32 kP_hastighed = .3;                 // Position p-gain
    INT16S output_hastighed = 0;            // output fra hastigheds-loopet

    static INT8U iteration_counter = 2;     // Antallet af gange de indre loop skal eksekveres pr. gang de ydre loop

    // Positions-loopet:
    if(iteration_counter == 2)
      {
        error_pos = destination - enc;      // Fejl-udregningen
        // Hvis positionen er indenfor +/- 5 sættes fejlen til 0:
        if (error_pos < 5 && error_pos > -5)
            error_pos = 0;
        // Hvis fejlen er indenfor +/- 35 aktiveres i-leddet
        if (error_pos < 35 && error_pos > -35)
        {
            integral_pos = integral_pos + (error_pos * iteration_time_pos);
        }
        // differential-leddet som umiddelbart ikke bruges.
        derivative_pos = (error_pos - last_error_pos)/iteration_time_pos;
        last_error_pos = error_pos;         // Anvendes som forrige fejl i næste iteration
        // Samlingen af alle led. Bestemmer outputtet af positions-loopet:
        output_pos = kP_pos * error_pos + kI_pos * integral_pos + kD_pos * derivative_pos;
        // Hvis fejlen bliver nul( som det gør indenfor +/- 5, nulstilles integrale-leddet, så der ikke skal et
        // overshoot til for at få det til 0:
        if ( error_pos == 0)
        {
            integral_pos = 0;
        }
        // Begrænsningen af hastigheden. Max-hastigheden er cirka 400. Hvis der kun reguleres på position, skal det
        // ændres til +/- 127, så der ikke skal overflow til PWM. Hvis der skal sættes en maksimum hastighed
        // skal det gøres her.

        if (output_pos > 400)
        {
            output_pos = 400;
        }
        else if (output_pos < -400)
        {
            output_pos = -400;
        }
        iteration_counter=0;                // Hver gang dette loop køres, skal iterations-counter nulstilles.
      }
    // Hastigheds-loopet:
    error_hastighed = output_pos - hastighed;
    output_hastighed = error_hastighed * kP_hastighed;
    iteration_counter++;

        if (output_hastighed > 127)
        {
            output_hastighed = 127;
        }
        else if (output_hastighed < -127)
        {
            output_hastighed = -127;
        }

    return output_hastighed;
}
/****************************** End Of Module *******************************/












