#ifndef AC_
#define AC_
#include <avr/io.h>

void AC_init(){
    SFIOR &= (1<<ACME); // set PB3 as negV
    ACSR &= 0x00; // enabling AC (ACD)
    // 00 << ACIS1..0 (on Toggle)
}

int AC_Result(){
    return (ACSR & (1<<ACO)); // return 1 if temp A (AIN0) is bigger than temp B (AIN1)
}

#endif