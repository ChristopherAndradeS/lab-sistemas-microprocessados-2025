/*
 * File:   ex01.c
 * Author: Aluno
 *
 * Created on 16 de Outubro de 2025, 10:49
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

    INTCON2bits.RBPU = 0; // Ativa resitores de pull-up da porta B
    ADCON1 = 0x0F;        // Defini pinos como entrada digital

    TRISB = 0xFF;         // Define porta B inteira como entrada
    TRISD = 0x00;         // Define porta D inteira como saída

   // Valor de PORTB é copiado para porta D
    while(1)
        LATD = PORTB;
    
    return;
}