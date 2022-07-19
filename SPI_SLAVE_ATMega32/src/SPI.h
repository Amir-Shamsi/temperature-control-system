#ifndef SPI_S_
#define SPI_S_
#include <avr/io.h>

char SPI_Receive()
{
	while (((SPSR >> SPIF) & 1) == 0); // wait until data recives
	return(SPDR);
}

void SPI_Init()
{
	DDRB = (0<<DDB7) | (1<<DDB6) | (0<<DDB5) | (0<<DDB4); // inputs and output

	SPCR = (1<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (1<<SPR1) | (1<<SPR0); // slave mode
    SPSR = (0<<SPI2X);
}

#endif