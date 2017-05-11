
#ifndef TIMER0A_H_
#define TIMER0A_H_

#include <stdint.h>
#include "swtimers.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "messages.h"


extern volatile INT16S ticks;

extern void init_timer0a(INT16U timer_value);
extern void TIMER0A_Handler(void);

#endif
