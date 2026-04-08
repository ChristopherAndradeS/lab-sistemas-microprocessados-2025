/*
 * File:   ex03.c
 * Author: Aluno
 *
 * Created on 16 de Outubro de 2025, 11:31
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

void main(void)
{
    INTCON2bits.RBPU = 0;
    ADCON1 = 0x0F;

    TRISB = 0xFF;
    TRISD = 0x00;
    PORTD = 0xFF;

    while(1)
    {
        if(!PORTBbits.RB7)
            LATDbits.LD0 = (PORTBbits.RB0 && PORTBbits.RB1);
        else
            LATDbits.LD0 = (PORTBbits.RB0 || PORTBbits.RB1);
    }

    return;
}
