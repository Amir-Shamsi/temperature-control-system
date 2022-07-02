#include <avr/io.h>
#include <util/delay.h>
#include <Lcd.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

int ADC_Read(char channel){
	ADMUX = 0x40 | (channel & 0x07);
	ADCSRA |= (1<<ADSC);
	while (!(ADCSRA & (1<<ADIF)));
	ADCSRA |= (1<<ADIF);
	return (int)((ADCW * 4.88) / 10.00);
}

void display_on_lcd(char* content, int delay){
  unsigned char i;
  for(i=0;i<(strlen(content));i++) {
        LCD_write(content[i]);
        if(delay)   
            _delay_ms(delay);
  }
}
void PWM_init()
{
	/*set fast PWM mode with non-inverted output*/
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (0<<COM00) | (1<<CS00);
	DDRB|=(1<<PB3);  /*set OC0 pin as output*/
}
char SPI_Receive()
{
	while (((SPSR >> SPIF) & 1) == 0);
	return(SPDR);
}

void SPI_Init()
{
	SPCR = (1<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (1<<SPR1) | (1<<SPR0);
    SPSR = (0<<SPI2X);
}


int main(void) {
    char r_temp;
    int base_duty_cycle = 50, additional_unit, final_duty_cycle;
    DDRC = 0xFF;
    DDRD = 0x07;
    DDRB = (0<<DDB7) | (1<<DDB6) | (0<<DDB5) | (0<<DDB4);
    init_LCD();
    // LCD_cmd(0x0F);
    SPI_Init();
    DDRD |= (1<<DDD3);

    sei();
    while (1) { 

          if ((int)r_temp > 55 || (int)r_temp < 25)
            TCCR0 = (0<<COM00) | (0<<COM01); // Turn off the cooler so disconnect OCO

          if ((int)r_temp >= 20)
            PORTB &= (0<<PORTB2);

          r_temp = SPI_Receive();

          if((int)r_temp > 55){ // if temp is higher than 55 degrees
            PORTD |= (1<<PORTD3);
            _delay_ms(100);
            PORTD &= (0<<PORTD3);
            _delay_ms(100);
          }

          else if ((int)r_temp < 20){
            DDRB |= (1<<DDB2);
            PORTB |= (1<<PB2);
          }
          else if ((int)r_temp <= 55 && (int)r_temp >= 25) { // For temperatures between 25 and 55 degrees
            PWM_init();
            additional_unit = (int)((r_temp - 25)/5);
            final_duty_cycle = base_duty_cycle + additional_unit*10;
            if(final_duty_cycle > 100)
              final_duty_cycle = 100;

            OCR0=(int)((final_duty_cycle*255)/100); 
          }

          else{ // Ideal state
            PORTD = (0<<PORTD3);
            // TODO: Turn off the cooler & heater
          }

    }
}