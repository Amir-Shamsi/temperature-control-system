#ifndef ADC_
#define ADC_
#include <avr/io.h>

void ADC_init(){    
	DDRA = 0x00; 
    
	ADCSRA = 0x87;
	//0xb 1 << ADEN 0000 111 << ADPS2..0 f/128
    
	ADMUX = 0x40;  
	//0xb01 << REFS1..0 000000   AVCC pin (5 V)
}

int ADC_Read(char pin){
	ADMUX = 0x40 | (pin & 0x07); // SiEn

	ADCSRA |= (1<<ADSC); // start

	while (!(ADCSRA & (1<<ADIF))); //don't start while ADC Interrupt Flag is 1

	ADCSRA |= (1<<ADIF); // make ADC Interrupt Flag is 1

	return (int)((ADCW * 4.88) / 10.00); // convertingto celsius
}

#endif