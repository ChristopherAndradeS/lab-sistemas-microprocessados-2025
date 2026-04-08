/*
 * File:   ex02.c
 * Author: Aluno
 *
 * Created on 16 de Outubro de 2025, 11:24
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

    LATD = 0x7F;

    while(1)
        LATDbits.LATD7 = PORTBbits.RB0;
    
    return;
}