/*
 * File:   ex01.c
 * Author: Aluno
 *
 * Created on 30 de Outubro de 2025, 10:55
 */

#include <xc.h>

#define _XTAL_FREQ 20000000

#pragma config CPUDIV = OSC1_PLL2
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config MCLRE = ON
#pragma config PWRT = ON
#pragma config XINST = OFF

signed char aux = 0;

void interrupt blink(void)
{
    if(PIR1bits.TMR1IF)
    {
       PIR1bits.TMR1IF = 0;
       LATCbits.LATC2 = ~LATCbits.LATC2;
       TMR1   = 63263;
    }

    if(INTCONbits.INT0IF)
    {
        __delay_ms(100);
        INTCONbits.INT0IF = 0;

        if(!T1CONbits.TMR1ON)
        {
            TMR1 = 63263;
            T1CONbits.TMR1ON = 1;
        }
        else
        {
            LATCbits.LATC2 = 0;
            T1CONbits.TMR1ON = 0;
            TMR1 = 0;
        }
    }
}
void main(void)
{
   INTCON2bits.RBPU = 0;   // habilita os resistores de pull-up internos
   ADCON1 = 0x0F;          // desliga entradas analógicas
   TRISB = 0xFF;
   TRISCbits.RC2 = 0;

   INTCON = 0b11010000;
   PIE1bits.TMR1IE  = 1;
   T1CON  =  0b10000101;
   TMR1   = 63263;

   while(1){}
}