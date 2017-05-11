/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: main.c
*
* PROJECT....: EMP
*
* DESCRIPTION: Assignment 1, main module. No main.h file.
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "swtimers.h"
#include "systick.h"
#include "button.h"
#include "button2.h"
#include "counter.h"
#include "countled.h"
#include "spi.h"

#include "gpio.h"

#include "events.h"

#include "uart.h"



/*****************************    Defines    *******************************/
#define Idle              0
#define Forward           1
#define Backward          2
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
extern INT16S ticks;
INT32U RX_SPI;
INT32U TX_SPI;

INT8S c;

INT8U var;

INT8U pwm = 0;


//INT8U motor_1_speed;
//INT8U motor_2_speed;
//INT8U motor_1_enc;
//INT8U motor_2_enc;


/*****************************   Functions   *******************************/


FP64 tToSpeed(INT16U val)
{
    INT16U calc_val;
    static INT16U avg1 = 0;
    static INT16U avg2 = 0;
    static INT16U avg3 = 0;
    static INT16U avg4 = 0;
    static INT16U avg5 = 0;
    static INT16U avg6 = 0;
//    static INT16U avg7 = 0;
//    static INT16U avg8 = 0;
//    static INT16U avg9 = 0;
//    static INT16U avg10 = 0;
//    static INT16U avg11 = 0;
//    static INT16U avg12 = 0;
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



INT16S uartReadChar(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : Read Char from UART0 data register
******************************************************************************/
{
//    while( (UART0_FR_R & UART_FR_RXFE) != 0  );  //Check if rx buffer is not empty
    INT16S c;
    c = UART0_DR_R;
    return c;
//    return (UART0_DR_R);
}


void uartWriteChar(INT16U c)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : Write up to 2 Chars to UART0 data register
******************************************************************************/

{
    while( (UART0_FR_R & UART_FR_TXFF) );  //Check if tx buffer is not empty
    UART0_DR_R = c;
}



void uartWriteStr(INT8U *character)
/*****************************************************************************
*   Input    : String
*   Output   :
*   Function : Write string throug h uart
******************************************************************************/
{
   while( *character != '\0' ){
      uartWriteChar( *character++ );
   }
}




//-----------------------UART_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 dig8its with no space before or after
void UART_OutUDec(INT32U n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10){
    UART_OutUDec(n/10);
    n = n%10;
  }
  uartWriteChar(n+'0'); /* n is between 0 and 9 */
}



/*
#define UART_FR_TXFE            0x00000080  // UART Transmit FIFO Empty
#define UART_FR_RXFF            0x00000040  // UART Receive FIFO Full
#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_FR_BUSY            0x00000008  // UART Busy
#define UART_FR_CTS             0x00000001  // Clear To Send

#define UART0_DR_R              (*((volatile uint32_t *)0x4000C000))
#define UART0_RSR_R             (*((volatile uint32_t *)0x4000C004))
#define UART0_ECR_R             (*((volatile uint32_t *)0x4000C004))
#define UART0_FR_R              (*((volatile uint32_t *)0x4000C018))
#define UART0_ILPR_R            (*((volatile uint32_t *)0x4000C020))
#define UART0_IBRD_R            (*((volatile uint32_t *)0x4000C024))
#define UART0_FBRD_R            (*((volatile uint32_t *)0x4000C028))
#define UART0_LCRH_R            (*((volatile uint32_t *)0x4000C02C))
#define UART0_CTL_R             (*((volatile uint32_t *)0x4000C030))
#define UART0_IFLS_R            (*((volatile uint32_t *)0x4000C034))
#define UART0_IM_R              (*((volatile uint32_t *)0x4000C038))
#define UART0_RIS_R             (*((volatile uint32_t *)0x4000C03C))
#define UART0_MIS_R             (*((volatile uint32_t *)0x4000C040))
#define UART0_ICR_R             (*((volatile uint32_t *)0x4000C044))
#define UART0_DMACTL_R          (*((volatile uint32_t *)0x4000C048))
#define UART0_9BITADDR_R        (*((volatile uint32_t *)0x4000C0A4))
#define UART0_9BITAMASK_R       (*((volatile uint32_t *)0x4000C0A8))
#define UART0_PP_R              (*((volatile uint32_t *)0x4000CFC0))
#define UART0_CC_R              (*((volatile uint32_t *)0x4000CFC8))
*/





INT16U readSPI(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : Initializing SSI2 for FreeScale SPI
******************************************************************************/
{
    INT16U RX_SPI;
    while(SSI2_SR_R & (1<<4));       //check if ss is active - active low
    RX_SPI = SSI2_DR_R;
    return RX_SPI;
}




void writeSPI(INT16U data)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : Initializing SSI2 for FreeScale SPI
******************************************************************************/
{
    INT16U datavar;
    INT16U datavar2;
    datavar = data;
    SSI2_DR_R = data;
    while( (SSI2_SR_R  & (1<<0)) == 0 ); //wait until tx buffer is empty

    datavar2 = readSPI();   // fjern under reg
//    UART_OutUDec(tToSpeed(datavar2));


    UART_OutUDec(datavar2); // fjern under reg

    uartWriteChar(9);       // Tab

    SSI2_DR_R = (3<<12) | 100;    // ask for speed
    UART_OutUDec(tToSpeed(readSPI()));

//                uartWriteChar('\n');    //for sampling
//                uartWriteChar('\r');


    uartWriteChar(9);       // Tab




 //       UART TX negative or positive;
        if(datavar & (1 << 7))
        {
            datavar &= 0xFF;
            datavar = datavar - 1;
            datavar ^= 0xFF;
            uartWriteChar('-');
            UART_OutUDec(datavar);
            uartWriteChar('\n');
            uartWriteChar('\r');
        }
        else
        {
            datavar &= 0xFF;
            UART_OutUDec(datavar);
            uartWriteChar('\n');
            uartWriteChar('\r');
        }


}


INT16U set_spd_get_cur_motor_1(INT8S speed_val) // speed val is +- 127
{
    INT16U speed1;
    speed1 = speed_val;
    speed1 &= 0xFF;     // Hvorfor
    speed1 |= (1<<12); // bit 14 motor 1, bit 13 motor 2
    return speed1;
}


INT16U get_cur_motor_1(INT8S speed_val) // speed val is +- 127
{
    INT16U speed1;
    speed1 = speed_val;
    speed1 &= 0xFF;     // Hvorfor
    speed1 |= (2<<12); // bit 14 motor 1, bit 13 motor 2
    return speed1;
}

INT16U get_spd_motor_1(INT8S speed_val) // speed val is +- 127
{
    INT16U speed1;
    speed1 = speed_val;
    speed1 &= 0xFF;     // Hvorfor
    speed1 |= (3<<12); // bit 14 motor 1, bit 13 motor 2
    return speed1;
}


INT16U get_enc_motor_1(INT8S speed_val) // speed val is +- 127
{
    INT16U speed1;
    speed1 = speed_val;
    speed1 &= 0xFF;     // Hvorfor
    speed1 |= (4<<12); // bit 14 motor 1, bit 13 motor 2
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

INT16U get_cur_motor_2(INT8S speed_val) // speed val is +- 127
{
    INT16U speed2;
    speed2 = speed_val;
    speed2 &= 0xFF;     // Hvorfor
    speed2 |= (6<<12); // bit 14 motor 1, bit 13 motor 2
    return speed2;
}

INT16U get_spd_motor_2(INT8S speed_val) // speed val is +- 127
{
    INT16U speed2;
    speed2 = speed_val;
    speed2 &= 0xFF;     // Hvorfor
    speed2 |= (7<<12); // bit 14 motor 1, bit 13 motor 2
    return speed2;
}

INT16U get_enc_motor_2(INT8S speed_val) // speed val is +- 127
{
    INT16U speed2;
    speed2 = speed_val;
    speed2 &= 0xFF;     // Hvorfor
    speed2 |= (8<<12); // bit 14 motor 1, bit 13 motor 2
    return speed2;
}

INT16U reset_motor_enc(void)
{
    INT16U reset = (15<<12);
 //   uartWriteStr("reset");
    return reset;
}

INT16U test(INT8S speed_val) // speed val is +- 127
{
    INT16U speed;
    speed = speed_val;
    speed &= 0xFF;     // Hvorfor
    speed |= (13<<12); // bit 14 motor 1, bit 13 motor 2
    return speed;
}




// Gammel protocol

//INT16U set_speed_motor_1(INT8S speed_val) // speed val is +- 127
//{
//    INT16U speed1;
//    speed1 = speed_val;
//    speed1 &= 0xFF;     // Hvorfor
//    speed1 |= (1<<14); // bit 14 motor 1, bit 13 motor 2
//    return speed1;
//}
//
//INT16U set_speed_motor_2(INT8S speed_val) // speed val is +- 127
//{
//    INT16U speed2;
//    speed2 = speed_val;
//    speed2 &= 0xFF;
//    speed2 |= (1<<13); // 14 motor 1, 13 motor 2
//    return speed2;
//}
//
//
//
//INT16U reset_motor_enc(void)
//{
//    INT16U reset = (1<<15);
// //   uartWriteStr("reset");
//    return reset;
//}










INT32U sw1(INT8U event)
{

    switch( event )
    {
        case BE_SINGLE_PUSH:
            uartWriteStr("STOP!");
            uartWriteChar('\n');
            uartWriteChar('\r');
//            writeSPI(get_enc_motor_1(30));
            writeSPI(set_spd_get_cur_motor_1(0));
            writeSPI(set_spd_get_cur_motor_2(0));
            break;
        case BE_DOUBBLE_PUSH:
            uartWriteStr("Double push");
            uartWriteChar('\n');
            uartWriteChar('\r');
            writeSPI(get_spd_motor_1(30));
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
    return RX_SPI;
}


//INT32U sw2(INT8U event)
//{
//    switch( event )
//    {
//        case BE_SINGLE_PUSH2:
//            writeSPI(set_speed_motor_1(-30));
// //           writeSPI(reset_motor_enc());
//        //    uartWriteStr("reset");
//            break;
//        case BE_DOUBBLE_PUSH2:
//            writeSPI(set_speed_motor_1(30));
//            break;
//        case BE_LONG_PUSH2:
//            writeSPI(set_speed_motor_2(30));
//            break;
//        default:
//            break;
//    }
//    return RX_SPI;
//}







//reg

INT8S goTo(INT16U enc, INT16U destination, INT16U hastighed) //distnation is enc val not degrees
{
    static FP32 last_error_pos = 0;
    static FP32 error_pos = 0;
    FP32 iteration_time_pos = 0.005;
    static FP32 integral_pos = 0;
    static FP32 derivative_pos = 0;
    FP32 kP_pos = 1;
    FP32 kI_pos = 0;
    FP32 kD_pos = 0;
    static INT16S output_pos = 0;
    static INT16U enc_last = 0;
    INT8S direction = 0;

    static FP32 error_hastighed = 0;
    FP32 kP_hastighed = 0.5;
    INT16S output_hastighed = 0;

    static INT8U iteration_counter = 5;

    if(enc >= enc_last)
        direction = 1;
    else
        direction = -1;

//    if(iteration_counter == 5)
//      {
        error_pos = destination - enc;
        integral_pos = integral_pos + (error_pos * iteration_time_pos);
        derivative_pos = (error_pos - last_error_pos)/iteration_time_pos;
        last_error_pos = error_pos;

        output_pos = (kP_pos * error_pos + kI_pos * integral_pos + kD_pos * derivative_pos);

        if ( error_pos == 0)
        {
            integral_pos = 0;
        }

        if (output_pos > 127)
        {
            output_pos = 127;
        }
        else if (output_pos < -127)
        {
            output_pos = -127;
        }
      //  iteration_counter=0;
//      }

    error_hastighed = output_pos - hastighed * direction;
    output_hastighed = error_hastighed * kP_hastighed;
    //iteration_counter++;
    enc_last = enc;
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

//
//INT8S goTo(INT8U enc, INT8U destination) //distnation is enc val not degrees
//{
//    static FP32 lastError = 0;
//    static FP32 error = 0;
//    static FP32 iterationTime = 0.005;
//    static FP32 integral = 0;
//    static FP32 derivative = 0;
//    static FP32 kP = 0.5;
//    static FP32 kI = 10;
//    static FP32 kD = 0.125;
//    static INT16S output = 0;
//
//
//    error = destination - enc;
//    integral = integral + (error * iterationTime);
//    derivative = (error - lastError)/iterationTime;
//    lastError = error;
//
//    output = (kP * error + kI * integral + kD * derivative);
//
//
//    if ( error == 0)
//    {
//        integral = 0;
//    }
//
//    if (output > 127)
//    {
//        output = 127;
//    }
//    else if (output < -127)
//    {
//        output = -127;
//    }
//
//
//
//
//    return output;
//}


void clr_rx_spi()
{
    INT16U dummy;
    while(SSI2_SR_R & (1<<3))
    {
        dummy = readSPI();
    }
}
int main(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{
    INT8U event1;
//    INT8U event2;
    INT8U alive_timer = TIM_500_MSEC;
    INT8U RX;
//    INT8U etsek;


    disable_global_int();
    init_systick();
    init_gpio();
    init_spi2();
    init_uart0( 9600, 8, 1, 'n' ); // 1 stop bit no parity


    enable_global_int();


  // Loop forever.
    while(1)
    {
    // System part of the super loop.
    // ------------------------------
	while( !ticks );

	// The following will be executed every 5mS
    ticks--;

	if( ! --alive_timer )
	{
	  alive_timer        = TIM_500_MSEC;
	  GPIO_PORTD_DATA_R ^= 0x40; //D
	}

	// Application part of the super loop.
	// -----------------------------------

	event1 = select_button();
//	event2 = select_button2();

//	uartWriteChar(event2);

	sw1(event1);

//	sw2(event2);

	c = uartReadChar();

/*
//  et sek test
	etsek++;
	if(etsek = 1)
	{
	    writeSPI(set_speed_motor_1(-60));
	}

	if(etsek >= 201)
	{
	    if (etsek = 201)
	    {
	        writeSPI(set_speed_motor_1(0));
	    }
	    etsek = 202;
	}

*/

	static INT16U enc = 0;
	static INT16U speed = 0;
	static INT16U destination = 1020; // 180 enc ticks

	static INT16U speed2;

	static INT16U var;

	switch(c)
	{
	    //REG test
	    case 'm':

	      clr_rx_spi();
        writeSPI( get_spd_motor_1(30));
        speed2 = readSPI();
        speed = tToSpeed(speed2);

        UART_OutUDec(speed);
        uartWriteChar(9);       // Tab

        clr_rx_spi();
        writeSPI( get_enc_motor_1(0));
        enc = readSPI();
        UART_OutUDec(enc);

        uartWriteChar('\n');
        uartWriteChar('\r');

        INT16U test = set_spd_get_cur_motor_1(30);
//        INT16U test = set_spd_get_cur_motor_1( goTo (  enc,  destination, speed  ) );
        writeSPI( test );
        var = readSPI();

	            break;
	    //Reset
	    case 'l':
	        writeSPI(reset_motor_enc());
	        RX = 7;
	        break;
	    //Stop motor 3
	    case ',':
            writeSPI(set_spd_get_cur_motor_1(0));
            RX = 0;
            break;
        //Stop motor 2
        case '.':
            writeSPI(set_spd_get_cur_motor_2(0));
            RX = 0;
            break;
	    //Motor2 speed
	    case 'a':
            writeSPI(set_spd_get_cur_motor_2(-15));
            RX = 1;
            break;
        case 's':
            writeSPI(set_spd_get_cur_motor_2(-30));
            RX = 2;
            break;
        case 'd':
            writeSPI(set_spd_get_cur_motor_2(-45));
            RX = 3;
            break;
        case 'f':
            writeSPI(set_spd_get_cur_motor_2(-60));
            RX = 4;
            break;
        case 'z':
            writeSPI(set_spd_get_cur_motor_2(15));
            RX = 1;
            break;
        case 'x':
            writeSPI(set_spd_get_cur_motor_2(30));
            RX = 2;
            break;
        case 'c':
            writeSPI(set_spd_get_cur_motor_2(45));
            RX = 3;
            break;
        case 'v':
            writeSPI(set_spd_get_cur_motor_2(60));
            RX = 4;
            break;
        //Motor1 speed
        case 'q':
            writeSPI(set_spd_get_cur_motor_1(-15));
            RX = 1;
            break;
        case 'w':
            writeSPI(set_spd_get_cur_motor_1(-30));
            RX = 2;
            break;
        case 'e':
            writeSPI(set_spd_get_cur_motor_1(-45));
            RX = 3;
            break;
        case 'r':
            writeSPI(set_spd_get_cur_motor_1(-60));
            RX = 4;
            break;
        case 't':
            writeSPI(set_spd_get_cur_motor_1(-75));
            RX = 5;
            break;
        case 'y':
            writeSPI(set_spd_get_cur_motor_1(-90));
            RX = 6;
            break;
        case 'u':
            writeSPI(set_spd_get_cur_motor_1(-105));
            RX = 7;
            break;
        case 'i':
            writeSPI(set_spd_get_cur_motor_1(-120));
            RX = 1;
            break;
        case 'o':
            writeSPI(set_spd_get_cur_motor_1(-127));
            RX = 2;
            break;


        case '1':
	        writeSPI(set_spd_get_cur_motor_1(15));
	        RX = 1;
	        break;
	    case '2':
	        writeSPI(set_spd_get_cur_motor_1(30));
            RX = 2;
            break;
	    case '3':
	        writeSPI(set_spd_get_cur_motor_1(45));
            RX = 3;
            break;
	    case '4':
	        writeSPI(set_spd_get_cur_motor_1(60));
	        RX = 4;
	        break;

/*
        case '1':
            writeSPI(set_speed_motor_1(32));
            RX = 1;
            break;
        case '2':
            writeSPI(set_speed_motor_1(63));
            RX = 2;
            break;
        case '3':
            writeSPI(set_speed_motor_1(95));
            RX = 3;
            break;
        case '4':
            writeSPI(set_speed_motor_1(127));
            RX = 4;
            break;
*/

        case '5':
	        writeSPI(set_spd_get_cur_motor_1(75));
            RX = 5;
            break;
        case '6':
            writeSPI(set_spd_get_cur_motor_1(90));
            RX = 6;
            break;
        case '7':
            writeSPI(set_spd_get_cur_motor_1(105));
            RX = 7;
            break;
        case '8':
            writeSPI(set_spd_get_cur_motor_1(120));
            RX = 1;
            break;
        case '9':
            writeSPI(set_spd_get_cur_motor_1(127));
            RX = 2;
            break;
        default:
            RX = 0;
            break;
        }

        counter_leds( RX );


    }
  return( 0 );
}

/****************************** End Of Module *******************************/
