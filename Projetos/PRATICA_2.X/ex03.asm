    #include <p18f4550.inc>

    config CPUDIV = OSC1_PLL2 ;
    config FOSC = HS ;
    config WDT = OFF ;
    config PBADEN = OFF ;
    config LVP = OFF ;
    config MCLRE = ON ;
    config PWRT = ON ;

    #define FAL 0x01
    #define FAH 0x00
    #define FBL 0x03
    #define FBH 0x02

    #define VAL 0x01    ; 0x0001
    #define VAH 0x00
    #define VBL 0xFA    ; 0x00FA
    #define VBH 0x00

    org 0x0000
    MOVLW VBL
    MOVWF FBL, 0
    MOVLW VBH
    MOVWF FBH, 0     ; Carrega VB em FB
    MOVLW VAL
    MOVWF FAL, 0
    MOVLW VAH
    MOVWF FAH, 0     ; Carrega VA em FA
    bra    start
    org 0x0020

start:
    MOVF FAL, W
    ADDWF FBL, F, 0  ; Soma da parte baixa

    MOVF FAH, W
    ADDWFC FBH, F, 0 ; Soma da parte alta (VA + VB + CARRY)

    bra    start
    end