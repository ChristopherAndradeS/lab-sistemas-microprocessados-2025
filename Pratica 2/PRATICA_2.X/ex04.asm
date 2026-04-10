    #include <p18f4550.inc>

    config CPUDIV = OSC1_PLL2 ;
    config FOSC = HS ;
    config WDT = OFF ;
    config PBADEN = OFF ;
    config LVP = OFF ;
    config MCLRE = ON ;
    config PWRT = ON ;

    #define VA 0x4A
    #define VB 0xA5
    #define FA 0x00
    #define FB 0x01
    #define FC 0x02

    org 0x0000
    bra    start
    org 0x020

start:

    ; Carregar variáveis
    MOVLW VA
    MOVWF FA, 0
    MOVLW VB
    MOVWF FB, 0

    MOVLW 0x0F      ; Mascara do nibble menos significativo
    ANDWF FA, W     ; Filtrage de bits
    MOVWF FC, 0     ; Resultado é jogado no endereço FC

    MOVLW 0xF0      ; Mascara do nibble mais significativo
    ANDWF FB, W     ; Filtrage de bits
    IORWF FC, F, 0  ; Concatenação de resultados no endereço FC
    
    bra    start
    end