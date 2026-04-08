    #include <p18f4550.inc>

    config CPUDIV = OSC1_PLL2 ;
    config FOSC = HS ;
    config WDT = OFF ;
    config PBADEN = OFF ;
    config LVP = OFF ;
    config MCLRE = ON ;
    config PWRT = ON ;

    #define VA 0x0A ; VA = 10
    #define VB 0x10 ; VB = 16
    #define FA 0x00
    #define FB 0x01
    #define FC 0x02

    org 0x0000
    bra start
    org 0x0020

start:

    ; Carregando variáveis
    MOVLW VA
    MOVWF FA, 0
    MOVLW VB
    MOVWF FB, 0

    RRCF FB, F      ; FB = FB / 2
    RRCF FB, F      ; FB = FB / 2
    RLCF FA, F, 0   ; FA = FA * 2
    MOVF FA, W      ; W <- FA
    ADDWF FB, W, 0  ; W = FA + FB
    MOVWF FC, 0     ; FC <- W

    bra    start
    end