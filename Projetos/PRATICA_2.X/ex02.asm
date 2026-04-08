    #include <p18f4550.inc>

    config CPUDIV = OSC1_PLL2 ;
    config FOSC = HS ;
    config WDT = OFF ;
    config PBADEN = OFF ;
    config LVP = OFF ;
    config MCLRE = ON ;
    config PWRT = ON ;

    ; Endereços de Memória  (A e B)
    #define FA 0x00
    #define FB 0x01
    ; Valores literais      (a e b)
    #define VA 0x01
    #define VB 0xEF

    org 0x0000
    bra    start
    org 0x0020

start:
    ; VB = VB + VA (VB += VA)
    ; 0xEF + 0x01 = 0xF0

    ;Caso particular (unsigned)
    ; (239) + (1) = 240

    ;Caso geral (singned)
    ; (-17) + (1) = -16

    MOVLW VB    ; Carregar VB em FB
    MOVWF FB, 0
    MOVLW VA    ; Carregar VA em FA
    MOVWF FA, 0
    ADDWF FB, F ; Somar VA + VB e guardar resultado em VB

    bra    start
    end