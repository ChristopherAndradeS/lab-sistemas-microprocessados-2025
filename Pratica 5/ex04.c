/*
 * File:   ex04.c
 * Author: Aluno
 *
 * Created on 16 de Outubro de 2025, 11:37
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

unsigned char count_bits();

void main(void)
{
    INTCON2bits.RBPU = 0;
    ADCON1 = 0x0F;

    TRISB = 0xFF;
    TRISD = 0x00;

    while(1)
        LATD = ~(count_bits());

    return;
}

unsigned char count_bits(void)
{
    unsigned char num = 0x00;

    for(unsigned char mask = 0x01; mask != 0; mask = mask << 1)
        if((~PORTB & mask) != 0)
            num += 0x01;

    return num;
}