/*
 * File:   ex01.c
 * Author: Aluno
 *
 * Created on 13 de Novembro de 2025, 12:01
 */

#include <xc.h>

#pragma config CPUDIV = OSC1_PLL2
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config MCLRE = ON
#pragma config PWRT = ON
#pragma config XINST = OFF

int count = 0;
char clock = 0;

char display7seg[10] =
{
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00100111,
    0b01111111,
    0b01101111,
};

void interrupt timer()
{
    if(INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;

        count += 1;

        if(count >= 1000)
        {
            count = 0;
            clock += 1;
                    
            if(clock >= 10)
                clock = 0;              
        }

        LATD = display7seg[clock];
        TMR0 = 65497;
    }
}

void main(void)
{
    INTCON2bits.RBPU = 0;
    ADCON1 = 0x0F;

    TRISA = 0x00;
    TRISE = 0x00;
    TRISD = 0x00;

    INTCON = 0b11100000;
    T0CON = 0x86;
    TMR0 = 65497;

    PORTEbits.RE0 = 0;
    PORTEbits.RE2 = 1;

    while(1){}
    
    return;
}

