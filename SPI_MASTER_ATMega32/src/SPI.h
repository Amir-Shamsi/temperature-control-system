#ifndef SPI_
#define SPI_
#include <avr/io.h>


void SPI_init(){
    DDRB = 0xB0; //0xb10110000 set PB4,5,7 as output and PB6 (MISO) as input
    PORTB |= (0<<PORTB4); // Make SS pin 1 (active low) for select slave
    SPCR = (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA) | (1<<SPR1) | (1<<SPR0);
    //   Enable SPI | MSB first |   Master  | clk 1 frst| rising clk| f/128 
    SPSR = (0<<SPI2X);
}

void SPI_Write(char data){
    // char ignore;
        SPDR = data;
        while(((SPSR >> SPIF) & 1) == 0); // wait until it's done
    // ignore = SPDR;
}

#endif