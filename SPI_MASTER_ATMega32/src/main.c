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
    
    /* ACD */
    SFIOR = (0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);
    DDRA = 0x00;
    ADCSRA = 0x87;
    ADMUX = 0x40;

    /* Analog Comparator */
    // SFIOR &= (1<<ACME);
    ACSR &= 0x00;

    sei();
    while (1) { 
      if (ACSR & (1<<ACO)){ // if temp A (AIN0) is bigger than temp B (AIN1)
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
      _delay_ms(200);
    }
}