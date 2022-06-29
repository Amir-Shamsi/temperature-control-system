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

char SPI_Receive()			/* SPI Receive data function */
{
	while(!(SPSR & (1<<SPIF)));	/* Wait till reception complete */
	return(SPDR);			/* Return received data */
}

void SPI_Init()					/* SPI Initialize function */
{
	DDRB = 0x40;  /* Make MOSI, SCK, SS as
 						input pins */
	// DDRB |= (1<<MISO);			/* Make MISO pin as 
						// output pin */
	SPCR = (1<<SPE);			/* Enable SPI in slave mode */
}

int main(void) {
    char fixed_text[20] = "Recieved Temp is ";
    char lcd_full_text[20], r_temp;
    DDRC = 0xFF;
    DDRD = 0x07;
    
    init_LCD();
    LCD_cmd(0x0F);
    SPI_Init();


    sei();
    while (1) { 
          r_temp = SPI_Receive();
          display_on_lcd(fixed_text, 0);
          LCD_cmd(0xC0);
          sprintf(lcd_full_text, "%d", );
          display_on_lcd(lcd_full_text, 0);
          LCD_cmd(0x01);
    }
}