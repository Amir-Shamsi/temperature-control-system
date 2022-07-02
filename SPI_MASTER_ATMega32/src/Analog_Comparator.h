#ifndef AC_
#define AC_
#include <avr/io.h>

void AC_init(){
    SFIOR &= (1<<ACME);
    ACSR &= 0x00;
}

int AC_Result(){
    return (ACSR & (1<<ACO));
}

#endif