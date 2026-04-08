    #include <p18f4550.inc>

    #define VA 0xF8
    #define VB 0x08
    #define VC 0x80
    #define VD 0xFF
    #define VE 0x00
    #define VF 0xFF
    #define VG 0xF6
    #define VH 0x0A

    config CPUDIV = OSC1_PLL2 ;
    config FOSC = HS ;
    config WDT = OFF ;
    config PBADEN = OFF ;
    config LVP = OFF ;
    config MCLRE = ON ;
    config PWRT = ON ;

    org 0x0000
    bra    start
    org 0X020

start:
    ; 1) a. CARRY / DIGITAL CARRY
    ; 0xF8 + 0x08 = 0x00
    ; (-8) + (8)  = 0
    MOVLW 0x00
    MOVWF STATUS, 0 ; Limpar STATUS para próxima operação
    MOVLW VA
    ADDLW VB

    ; 2) b. OVERFLOW
    ; 0x80 + 0xFF = 0x7F
    ; (-127 + (-1) = (-128) -> overflow -> (127)
    MOVLW 0x00
    MOVWF STATUS, 0 ; Limpar STATUS para próxima operação
    MOVLW VC
    ADDLW VD

    ; 3) c. NEGATIVO
    ; 0x00 + 0xFF = 0xFF
    ; (0) + (-1) = (-1)
    MOVLW 0x00
    MOVWF STATUS, 0 ; Limpar STATUS para próxima operação
    MOVLW VE
    ADDLW VF

    ; 4) d. ZERO
    ; 0xF6 + 0x0A = 0x00
    ; (-10) + (10) = (0)
    MOVLW 0x00
    MOVWF STATUS, 0 ; Limpar STATUS para próxima operação
    MOVLW VG
    ADDLW VH

    bra    start
    end