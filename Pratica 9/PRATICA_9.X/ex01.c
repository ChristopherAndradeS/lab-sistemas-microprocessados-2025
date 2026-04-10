#include <xc.h>

#pragma config CPUDIV = OSC1_PLL2
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config MCLRE = ON
#pragma config PWRT = ON
#pragma config XINST = OFF

#define _XTAL_FREQ 5000000

int main()
{
    ADCON0 = 0x01;
    ADCON1 = 0x0E;
    ADCON2 = 0x21;
    CCP2CON = 0x0C;
    T2CON = 0b00000101;
    PR2 = 255;
    TRISC = 0x00;
    TRISA = 0xFF;

    while(1)
    {
        ADCON0bits.GO = 1;

        while(ADCON0bits.GO);

        CCPR2L = ADRESH;

        __delay_us(10);
    }

    return;
}