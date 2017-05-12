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
#include "events.h"
#include "protokol.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/
void UI_(INT8U c)
{
    static INT16U enc = 0;
      static INT16U speed = 0;
      static INT16U destination = 1020; // 180 enc ticks

      static INT16U speed2;

      static INT16U var;

switch(c)
  {
      //REG test
      case 'm':
//      clr_rx_spi();
//      writeSPI(get_enc_motor_1());
//      enc = readSPI();
      clr_rx_spi();
      writeSPI(get_spd_motor_1() | 3);
      speed = tToSpeed(readSPI());
      writeSPI(set_spd_get_cur_motor_1(goTo(900-1,destination,0)));

      break;
      //Reset
      case 'l':
          writeSPI(reset_motor_enc());
          break;
      //Stop motor 3
      case ',':
            writeSPI(set_spd_get_cur_motor_1(0));
            break;
        //Stop motor 2
        case '.':
            writeSPI(set_spd_get_cur_motor_2(0));
            break;
      //Motor2 speed
      case 'a':
            writeSPI(set_spd_get_cur_motor_2(-15));
            break;
        case 's':
            writeSPI(set_spd_get_cur_motor_2(-30));
            break;
        case 'd':
            writeSPI(set_spd_get_cur_motor_2(-45));
            break;
        case 'f':
            writeSPI(set_spd_get_cur_motor_2(-60));
            break;
        case 'z':
            writeSPI(set_spd_get_cur_motor_2(15));
            break;
        case 'x':
            writeSPI(set_spd_get_cur_motor_2(30));
            break;
        case 'c':
            writeSPI(set_spd_get_cur_motor_2(45));
            break;
        case 'v':
            writeSPI(set_spd_get_cur_motor_2(60));
            break;
        //Motor1 speed
        case 'q':
            writeSPI(set_spd_get_cur_motor_1(-15));
            break;
        case 'w':
            writeSPI(set_spd_get_cur_motor_1(-30));
            break;
        case 'e':
            writeSPI(set_spd_get_cur_motor_1(-45));
            break;
        case 'r':
            writeSPI(set_spd_get_cur_motor_1(-60));
            break;
        case 't':
            writeSPI(set_spd_get_cur_motor_1(-75));
            break;
        case 'y':
            writeSPI(set_spd_get_cur_motor_1(-90));
            break;
        case 'u':
            writeSPI(set_spd_get_cur_motor_1(-105));
            break;
        case 'i':
            writeSPI(set_spd_get_cur_motor_1(-120));
            break;
        case 'o':
            writeSPI(set_spd_get_cur_motor_1(-127));
            break;


        case '1':
          writeSPI(set_spd_get_cur_motor_1(15));
          break;
      case '2':
          writeSPI(set_spd_get_cur_motor_1(30));
            break;
      case '3':
          writeSPI(set_spd_get_cur_motor_1(45));
            break;
      case '4':
          writeSPI(set_spd_get_cur_motor_1(60));
          break;
        case '5':
          writeSPI(set_spd_get_cur_motor_1(75));
            break;
        case '6':
            writeSPI(set_spd_get_cur_motor_1(90));
            break;
        case '7':
            writeSPI(set_spd_get_cur_motor_1(105));
            break;
        case '8':
            writeSPI(set_spd_get_cur_motor_1(120));
            break;
        case '9':
            writeSPI(set_spd_get_cur_motor_1(127));
            break;
        default:
            break;
        }

    }

void sw1(INT8U event)
{

    switch( event )
    {
        case BE_SINGLE_PUSH:
            uartWriteStr("STOP!");
            uartWriteChar('\n');
            uartWriteChar('\r');
//            writeSPI(get_enc_motor_1(30));
            writeSPI(set_spd_get_cur_motor_1(30));
//            writeSPI(set_spd_get_cur_motor_2(0));
            break;
        case BE_DOUBBLE_PUSH:
            uartWriteStr("Double push");
            uartWriteChar('\n');
            uartWriteChar('\r');
//            writeSPI(get_spd_motor_1());
            writeSPI(set_spd_get_cur_motor_1(60));
//            writeSPI(test(30));
//            writeSPI(set_spd_get_cur_motor_1(-30));
            break;
        case BE_LONG_PUSH:
            writeSPI(reset_motor_enc());
            uartWriteStr("reset!");
            uartWriteChar('\n');
            uartWriteChar('\r');
            break;
        default:
            break;
    }

}

/****************************** End Of Module *******************************/

















