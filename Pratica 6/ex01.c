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
    if(INTCONbits.TMR0IF)
    {
       INTCONbits.TMR0IF = 0;

       aux = LATEbits.LATE0;
       LATEbits.LATE0 = LATEbits.LATE1;
       LATEbits.LATE1 = aux;
       TMR0   = 26474;
    }

    if(INTCONbits.INT0IF)
    {
        __delay_ms(100);
        INTCONbits.INT0IF = 0;

        if(!T0CONbits.TMR0ON)
        {
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 0;
            TMR0 = 26474;
            T0CONbits.TMR0ON = 1;
        }
        else
        {
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 0;
            TMR0 = 0;
            T0CONbits.TMR0ON = 0;
        }
    }
}
void main(void)
{
   INTCON2bits.RBPU = 0;   // habilita os resistores de pull-up internos
   ADCON1 = 0x0F;          // desliga entradas analógicas
   TRISB = 0xFF;
   TRISEbits.RE0 = 0;
   TRISEbits.RE1 = 0;
   LATEbits.LATE0 = 1;
   LATEbits.LATE1 = 0;

   INTCON = 0b11110000;
   T0CON  = 0x86;
   TMR0   = 26474;
  
   while(1){}
}