 #include "timer0A.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

volatile INT16S ticks;

#define TIMER_VALUE_MS(timer_value_ms)  16000000/(1000/timer_value_ms)

void init_timer0a(INT16U timer_value_ms)
{
// Timer setup:
    SYSCTL_RCGCTIMER_R |= (1<<0); // Setting bit 0 to 1, to enable timer 0.
    TIMER0_CTL_R  &= ~(1<<0); // Disable the control register for timer 0 A.
    TIMER0_CFG_R = 0x00000000;   // Clear the configuration register
    TIMER0_TAMR_R |= (0b10<<0);   // Configure the time to run in perodic mode
    TIMER0_TAMR_R &= ~(1<<4); // Setting the direction to 0, to count down from a value
    TIMER0_TAILR_R = TIMER_VALUE_MS(timer_value_ms); // Setting the count down value to 16.000.000/5 = 3.200.000

    TIMER0_IMR_R |= (1<<0); // Enable interrupt for timer 0 A.
    NVIC_EN0_R |= (1<<19); // Enable

    TIMER0_CTL_R |= (1<<0); // Enable timer 0 A, after setup.
}

void TIMER0A_Handler(void)
{
    TIMER0_ICR_R |= (1<<0);
    ticks++;
}
