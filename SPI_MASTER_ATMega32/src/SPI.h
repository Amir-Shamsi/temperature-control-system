#ifndef SPI_
#define SPI_
#include <avr/io.h>


void SPI_init(){
    DDRB =0xB0;
    PORTB |= (0<<PORTB4);			/* Make high on SS pin */
    SPCR = (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA) | (1<<SPR1) | (1<<SPR0);
    SPSR = (0<<SPI2X);    
}

void SPI_Write(char data){
    // char ignore;
        SPDR = data;
        while(((SPSR >> SPIF) & 1) == 0);
    // ignore = SPDR;
}

#endif