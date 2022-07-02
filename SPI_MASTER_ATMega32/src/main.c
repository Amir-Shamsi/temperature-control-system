#include <avr/io.h>
#include <util/delay.h>
#include <Lcd.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "SPI.h"
#include "ADC.h"
#include "Analog_Comparator.h"


void LCD_String_Write(char* content){
  unsigned char i;
  for(i=0; i<(strlen(content)); i++) {
        LCD_write(content[i]);   
        // _delay_ms(delay);
  }
}


int main(void) {
    char init_text[20] = "Temperature A:";
    char lcd_full_text[20];
    int temp_A, pre_temp_A, freeze_flag=0;
    DDRC = 0xFF;
    DDRD = 0x07;
    
    init_LCD();
    SPI_init();
    ADC_init();
    AC_init();

    sei();
    while (1) { 
      if (AC_Result()){ // if temp A (AIN0) is bigger than temp B (AIN1)
        temp_A = ADC_Read(0);
        SPI_Write(temp_A);

        if(temp_A != pre_temp_A){
          LCD_cmd(0x01);
          
          pre_temp_A = temp_A;
          sprintf(lcd_full_text, "%d%cC", temp_A, 0xdf);
          LCD_String_Write(init_text);
          LCD_cmd(0xC0);
          LCD_String_Write(lcd_full_text);
          freeze_flag = 0;
        }
     }
      else {
        if(!freeze_flag){
          freeze_flag = 1;
          LCD_cmd(0x01);

          sprintf(lcd_full_text, "Temp A < Temp B");
          LCD_String_Write(lcd_full_text);
        }
      }
      _delay_ms(2000);
    }
}