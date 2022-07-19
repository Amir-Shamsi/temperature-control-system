#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "SPI.h"
#include "PWM.h"


void display_on_lcd(char* content, int delay){
  unsigned char i;
  for(i=0;i<(strlen(content));i++) {
        LCD_write(content[i]);
        if(delay)   
            _delay_ms(delay);
  }
}


int main(void) {
    char r_temp;
    int base_duty_cycle = 50, additional_unit, final_duty_cycle;
    // DDRC = 0xFF;
    // DDRD = 0x07; 
    
    SPI_Init();
    DDRD |= (1<<DDD3); // for warning LED

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

            OCR0 = (int)((final_duty_cycle*255)/100); 
          }

          else{ // Ideal state
            PORTD = (0<<PORTD3);
          }

    }
}