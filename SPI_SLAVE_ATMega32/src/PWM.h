#ifndef PWM_S_
#define PWM_S_
#include <avr/io.h>

void PWM_init()
{
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (0<<COM00) | (1<<CS00);
	//         Fast PWM	mode		|      non-inverting      |   clk/1 
	DDRB|=(1<<PB3);  // set OC0 (PB3) pin as output
}

#endif