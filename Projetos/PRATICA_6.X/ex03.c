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

void __interrupt(high_priority) High(void)
{
    // Timer0: pisca LED
    if (INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;

        aux = LATEbits.LATE0;
        LATEbits.LATE0 = LATEbits.LATE1;
        LATEbits.LATE1 = aux;

        TMR0 = 26474;
    }

    // INT0: pausar/reiniciar pisca LED
    if (INTCONbits.INT0IF)
    {
        __delay_ms(100);
        INTCONbits.INT0IF = 0;

        if (!T0CONbits.TMR0ON)
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

// --------------------------
//  BAIXA PRIORIDADE
// --------------------------
void __interrupt(low_priority) Low(void)
{
    // Timer1: buzzer
    if (PIR1bits.TMR1IF)
    {
        PIR1bits.TMR1IF = 0;
        LATCbits.LATC2 = ~LATCbits.LATC2;
        TMR1 = 63263;
    }

    // INT1: ligar/desligar buzzer
    if (INTCON3bits.INT1IF)
    {
        __delay_ms(100);
        INTCON3bits.INT1IF = 0;

        if (!T1CONbits.TMR1ON)
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
   TRISEbits.RE0 = 0;
   TRISEbits.RE1 = 0;
   LATEbits.LATE0 = 1;
   LATEbits.LATE1 = 0;
   TRISCbits.RC2 = 0;

   RCONbits.IPEN = 1;

   INTCON2bits.TMR0IP = 1;

   IPR1bits.TMR1IP = 0;
   INTCON3bits.INT1IP = 0;

   INTCON = 0b11110000;
   INTCON3bits.INT1IE = 1;
   PIE1bits.TMR1IE  = 1;
   T0CON  = 0x86;
   T1CON  =  0b10000101;
   TMR0   = 26474;
   TMR1   = 63263;

   while(1){}
}