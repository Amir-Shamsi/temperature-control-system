#include <avr/io.h>
#include <util/delay.h>
#include <Lcd.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "ADC.h"
#include "Analog_Comparator.h"


void display_on_lcd(char* content, int delay){
  unsigned char i;
  for(i=0;i<(strlen(content));i++) {
        LCD_write(content[i]);   
        // _delay_ms(delay);
  }
}

void SPI_Init()					/* SPI Initialize function */
{
	DDRB =0xB0;
	PORTB |= (0<<PORTB4);			/* Make high on SS pin */
  SPCR = (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA) | (1<<SPR1) | (1<<SPR0);
  SPSR = (0<<SPI2X);
}

void SPI_Write(char data)		/* SPI write data function */
{
  // char ignore;
	SPDR = data;
	while(((SPSR >> SPIF) & 1) == 0);
  // ignore = SPDR;
}

int main(void) {
    char init_text[20] = "Temperature A:";
    char lcd_full_text[20];
    int tempA, pre_tempA, freeze_flag=0;
    DDRC = 0xFF;
    DDRD = 0x07;
    
    init_LCD();
    SPI_Init();
    

    ADC_init();
    AC_init();


    sei();
    while (1) { 
      if (AC_Result()){ // if temp A (AIN0) is bigger than temp B (AIN1)
        tempA = ADC_Read(0);
        SPI_Write(tempA);


        if(tempA != pre_tempA){
          LCD_cmd(0x01);
          
          pre_tempA = tempA;
          sprintf(lcd_full_text, "%d%cC", tempA, 0xdf);
          display_on_lcd(init_text, 50);
          LCD_cmd(0xC0);
          display_on_lcd(lcd_full_text, 50);
          freeze_flag = 0;
        }
     }
      else {
        if(!freeze_flag){
          freeze_flag = 1;
          LCD_cmd(0x01);

          sprintf(lcd_full_text, "Temp A < Temp B");
          display_on_lcd(lcd_full_text, 50);
        }
      }
      _delay_ms(2000);
    }
}