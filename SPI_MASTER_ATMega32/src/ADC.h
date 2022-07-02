#ifndef ADC_
#define ADC_
#include <avr/io.h>

void ADC_init(){
    SFIOR = (0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);
    DDRA = 0x00;
    ADCSRA = 0x87;
    ADMUX = 0x40;
}

int ADC_Read(char pin){
	ADMUX = 0x40 | (pin & 0x07);
	ADCSRA |= (1<<ADSC);
	while (!(ADCSRA & (1<<ADIF)));
	ADCSRA |= (1<<ADIF);
	return (int)((ADCW * 4.88) / 10.00);
}

#endif