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

int GetMinutes(char num)
{
    return (int)(num / 60);
}

int GetSeconds(char num)
{
    return (int)(num % 60) % 60;
}

int GetUnidade(char num)
{
    return (int) num % 10;
}

int GetDezena(char num)
{
    return (int) num / 10;
}

void interrupt timer()
{
    if(INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;

        if(PORTAbits.RA5)
        {
            LATD = display7seg[GetUnidade(GetSeconds(clock))];
            LATEbits.LATE0  = 0;
            LATEbits.LATE2  = 1;
            LATAbits.LA2    = 0;
            LATAbits.LA5    = 0;
        }

        else if(PORTEbits.RE2)
        {
            LATD = display7seg[GetDezena(GetSeconds(clock))];
            LATEbits.LATE0  = 1;
            LATEbits.LATE2  = 0;
            LATAbits.LA2    = 0;
            LATAbits.LA5    = 0;
        }

        else if(PORTEbits.RE0)
        {

            LATD = display7seg[GetUnidade(GetMinutes(clock))];
            LATEbits.LATE0  = 0;
            LATEbits.LATE2  = 0;
            LATAbits.LA2    = 1;
            LATAbits.LA5    = 0;
        }

        else
        {
            LATD = display7seg[GetDezena(GetMinutes(clock))];
            LATEbits.LATE0  = 0;
            LATEbits.LATE2  = 0;
            LATAbits.LA2    = 0;
            LATAbits.LA5    = 1;
        }

        count += 1;

        if(count >= 1000)
        {
            count = 0;
            clock += 1;
            if(clock >= 3600)
                clock = 0;
        }

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

    PORTEbits.RE0 = 1;
    PORTEbits.RE2 = 1;
    PORTAbits.RA2 = 1;
    PORTAbits.RA5 = 1;

    while(1){}

    return;
}

