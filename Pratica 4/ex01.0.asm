    #include <p18f4550.inc>

    #define VA 0x11     ; Valor de VA
    #define VB 0x03     ; Valor de VB
    #define FA 0x00     ; Endereço de VA
    #define FB 0x01     ; Endereço de VB
    #define FC 0x02     ; Endereço do resultado

    config CPUDIV = OSC1_PLL2 ;
    config FOSC = HS ;
    config WDT = OFF ;
    config PBADEN = OFF ;
    config LVP = OFF ;
    config MCLRE = ON ;
    config PWRT = ON ;

    org 0x0000
    bra    start
    org 0x0020

start:
    MOVLW VA        ; Carrega VA
    MOVWF FA, 0
    MOVLW VB        ; Carrega VB
    MOVWF FB, 0
    CLRF  FC, 0     ; Carrega FC (resultado)
    
    MOVF FA, F      ; Se FA = 0, código encerra
    BZ out
    MOVF FB, F      ; Se FB = 0, código encerra
    BZ out

    bra operate     ; se FA != 0 ^ FB != 0, operamos

operate:
    MOVF FA, W      ; W <- FA (para comparação na intrução seguinte)
    CPFSGT FB       ; FA > FB ?
    bra mul_VA_VB   ; Multiplicar Somando VA e decrementando VB (VB <= VA)
    bra mul_VB_VA   ; Multiplicar Somando VB e decrementando VA (VA < VB)
mul_VA_VB:
    ADDWF FC, F     ; FC <- FC + FA
    DECF FB         ; FB <- FB - 1
    BNZ mul_VA_VB   ; para quando FB ficar zerado
    bra out
mul_VB_VA:
    MOVF FB, W
    ADDWF FC, F     ; FC <- FC + FB
    DECF FA         ; FA <- FA - 1
    BNZ mul_VB_VA   ; para quando FA ficar zerado
out:
    bra out
    end